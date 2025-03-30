#include <stdio.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"
#include "led.h"

static const char *TAG = "example";

// see: https://github.com/espressif/esp-idf/issues/12592#issuecomment-1817343243
#define BLINK_GPIO (gpio_num_t) CONFIG_BLINK_GPIO

#define BREATHE_SPEED 0.01  // 呼吸速度（值越大呼吸越快）
#define BREATHE_DELAY_MS 25 // 刷新间隔（影响流畅度）
#define SATURATION 0.9      // 饱和度 (0.0~1.0)
#define PHASE_INCREMENT 180 // 控制呼吸速度（246 ≈ 2π/256）

static led_strip_handle_t led_strip;

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

    uint16_t phase = 0;       // 相位累加器
    uint16_t hue = 0;         // 色相（0-359）
    uint8_t saturation = 255; // 饱和度（0-255）
    while (1)
    {
        int32_t sin_val = int_sin(phase);
        uint8_t val = (sin_val * 255L) >> 16; // 转换为0-255范围
        val = (val + 256) >> 1;               // 修正为单向波形
        uint8_t r, g, b;
        hsv2rgb_fast(hue, saturation, 20, &r, &g, &b);

        ESP_LOGI(TAG, "changing color to rgb=%d,%d,%d!", r, g, b);
        led_strip_set_pixel(led_strip, 0, r, g, b);
        led_strip_refresh(led_strip);

        // 相位和色相更新
        phase += PHASE_INCREMENT; // 控制呼吸速度
        hue = (hue + 1) % 360;    // 色相渐变（可选）
        vTaskDelay(pdMS_TO_TICKS(BREATHE_DELAY_MS));
    }
}