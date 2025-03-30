#include <stdio.h>
#include <math.h>
#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <driver/i2s.h>
#include <esp_log.h>
#include <led_strip.h>

#include "sdkconfig.h"
#include "fast_hsv2rgb.h"
#include "wav.h"

static const char *TAG = "example";

// see: https://github.com/espressif/esp-idf/issues/12592#issuecomment-1817343243
#define BLINK_GPIO (gpio_num_t) CONFIG_BLINK_GPIO
#define I2S_BCLK 15
#define I2S_LRC 16
#define I2S_DOUT 7

#define I2S_MIC_WS   4   // WS (LRCLK)
#define I2S_MIC_BCLK 5 // BCLK
#define I2S_MIC_DATA 6 // DATA

const float PI = 3.14159265f;
const float HUE_SPEED = 3.0f;      // 色相变化速度（度/循环周期）
const float BREATHE_SPEED = 0.03f; // 呼吸速度（相位增量/循环周期）

static led_strip_handle_t led_strip;

static void configure_speaker()
{
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
        .sample_rate = 44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = true};

    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCLK,
        .ws_io_num = I2S_LRC,
        .data_out_num = I2S_DOUT,
        .data_in_num = I2S_PIN_NO_CHANGE};

    ESP_ERROR_CHECK(i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL));
    ESP_ERROR_CHECK(i2s_set_pin(I2S_NUM_0, &pin_config));
}

void configure_mic()
{
    // I2S 配置
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX), // 主模式，输入
        .sample_rate = 44100,                                // 采样率（Hz）
        .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,        // INMP441 输出 24 位数据，需用 32 位接收
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,         // 单声道（左）
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,   // 标准 I2S 格式
        .intr_alloc_flags = 0,
        .dma_buf_count = 8, // DMA 缓冲区数量
        .dma_buf_len = 64,  // 缓冲区长度
        .use_apll = false,  // 禁用 APLL
        .tx_desc_auto_clear = false,
        .mclk_multiple = I2S_MCLK_MULTIPLE_256 // 主时钟分频
    };

    // 引脚配置
    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_MIC_BCLK,
        .ws_io_num = I2S_MIC_WS,
        .data_out_num = I2S_PIN_NO_CHANGE, // 未使用输出引脚
        .data_in_num = I2S_MIC_DATA        // 数据输入引脚
    };

    // 初始化 I2S
    ESP_ERROR_CHECK(i2s_driver_install(I2S_NUM_1, &i2s_config, 0, NULL));
    ESP_ERROR_CHECK(i2s_set_pin(I2S_NUM_1, &pin_config));
}

void read_sound()
{
    int32_t buffer[128]; // 存储音频数据的缓冲区
    size_t bytes_read;

    // 从 I2S 读取数据
    i2s_read(I2S_NUM_1, buffer, sizeof(buffer), &bytes_read, portMAX_DELAY);

    // 处理数据（示例：打印第一个采样值）
    int32_t sample = buffer[0] >> 8; // INMP441 的有效数据在高 24 位
    ESP_LOGI(TAG, "read data from mic!");
}

void generate_test_sound()
{
    size_t bytes_written;
    for (uint32_t i = 0; i < audio_length; i++)
    {
        // 从Flash读取数据（无需pgmspace.h）
        int16_t sample;
        memcpy(&sample, &audio_data[i], sizeof(sample)); // 安全读取方式

        // 发送立体声数据
        int16_t stereo_sample[2] = {sample, sample};
        i2s_write(I2S_NUM_0, stereo_sample, sizeof(stereo_sample), &bytes_written, portMAX_DELAY);
    }
}

static void configure_led(void)
{
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 1,
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags = {.with_dma = true},
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    led_strip_clear(led_strip);
}

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "ESP32 agent running!");
    configure_led();
    configure_speaker();
    configure_mic();

    while (1)
    {
        // generate_test_sound();
        read_sound();
    }

    uint16_t hue = 0;         // 色相值（0-360）
    uint8_t saturation = 255; // 固定最大饱和度
    float brightness = 0;     // 亮度值（0-255）
    float phase = 0;          // 呼吸相位

    while (1)
    {
        brightness = (sin(phase) + 1) * 127.5f;
        uint8_t r, g, b;
        fast_hsv2rgb_32bit(hue, saturation, (uint8_t)roundf(brightness), &r, &g, &b);
        // TODO: gammar correction

        // for single color led, we can also use `ledc_set_fade_with_time`
        led_strip_set_pixel(led_strip, 0, r, g, b);
        led_strip_refresh(led_strip);

        // 更新色相和呼吸相位
        hue = (uint16_t)(hue + HUE_SPEED) % HSV_HUE_MAX;
        phase = fmod(phase + BREATHE_SPEED, 2 * PI);
        vTaskDelay(pdMS_TO_TICKS(40));
    }
}