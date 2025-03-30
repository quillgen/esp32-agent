#include <stdio.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"
#include "fast_hsv2rgb.h"

static const char *TAG = "example";

// see: https://github.com/espressif/esp-idf/issues/12592#issuecomment-1817343243
#define BLINK_GPIO (gpio_num_t) CONFIG_BLINK_GPIO

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

    uint16_t hue = 0;         // 色相值（0-360）
    uint8_t saturation = 255; // 固定最大饱和度
    float brightness = 0;     // 亮度值（0-255）
    float phase = 0;          // 呼吸相位

    const float PI = 3.14159265f;
    const float HUE_SPEED = 3.0f;      // 色相变化速度（度/循环周期）
    const float BREATHE_SPEED = 0.03f; // 呼吸速度（相位增量/循环周期）

    while (1)
    {
        brightness = (sin(phase) + 1) * 127.5f;
        uint8_t r, g, b;
        fast_hsv2rgb_32bit(hue, saturation, (uint8_t)roundf(brightness), &r, &g, &b);

        led_strip_set_pixel(led_strip, 0, r, g, b);
        led_strip_refresh(led_strip);

        // 更新色相和呼吸相位
        hue = (uint16_t)(hue + HUE_SPEED) % HSV_HUE_MAX;
        phase = fmod(phase + BREATHE_SPEED, 2 * PI);
        vTaskDelay(pdMS_TO_TICKS(40));
    }
}