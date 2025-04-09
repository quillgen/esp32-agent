#include "led/rgb_led.h"
#include "application.h"
#include "led/fast_hsv2rgb.h"
#include <esp_log.h>
#include <math.h>
#include <stdio.h>

using namespace walle;

#define TAG "RgbLed"

const float PI = 3.14159265f;
const float HUE_SPEED = 3.0f;      // 色相变化速度（度/循环周期）
const float BREATHE_SPEED = 0.03f; // 呼吸速度（相位增量/循环周期）

rgb_led::rgb_led(gpio_num_t pin) : pin(pin) {
  led_strip_config_t strip_config = {
      .strip_gpio_num = this->pin,
      .max_leds = 1,
      .led_model = LED_MODEL_WS2812,
      .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
  };
  led_strip_rmt_config_t rmt_config = {
      .resolution_hz = 10 * 1000 * 1000, // 10MHz
      .flags =
          {
              // If the RMT hardware can't be assist by DMA, the driver will
              // going into interrupt very frequently, thus result in a high CPU
              // usage.
              .with_dma =
                  true, // DMA feature is available on chips like ESP32-S3/P4
          },
  };
  ESP_ERROR_CHECK(
      led_strip_new_rmt_device(&strip_config, &rmt_config, &this->led_strip));
  led_strip_clear(this->led_strip);

  esp_timer_create_args_t strip_timer_args = {
      .callback =
          [](void *arg) {
            auto strip = static_cast<rgb_led *>(arg);
            std::lock_guard<std::mutex> lock(strip->mutex);
            if (strip->strip_callback != nullptr) {
              strip->strip_callback();
            }
          },
      .arg = this,
      .dispatch_method = ESP_TIMER_TASK,
      .name = "strip_timer",
      .skip_unhandled_events = false,
  };
  ESP_ERROR_CHECK(esp_timer_create(&strip_timer_args, &this->strip_timer));
}

rgb_led::~rgb_led() {
  esp_timer_stop(this->strip_timer);
  if (this->led_strip != nullptr) {
    led_strip_del(this->led_strip);
  }
}

void rgb_led::on_state_changed() {
  auto &app = application::instance();
  auto state = app.get_state();
  ESP_LOGI(TAG, "Received app state change: %d", state);
  switch (state) {
  case starting:
    // breathe(20);
    led_strip_set_pixel(this->led_strip, 0, 1, 1, 1);
    led_strip_refresh(this->led_strip);
    break;

  default:
    break;
  }
}

void rgb_led::show(uint8_t r, uint8_t g, uint8_t b) {
  std::lock_guard<std::mutex> lock(this->mutex);
  ESP_LOGI(TAG, "Set led to rgb mode");
  led_strip_set_pixel(this->led_strip, 0, r, g, b);
  led_strip_refresh(this->led_strip);
}

void rgb_led::blink(uint8_t r, uint8_t g, uint8_t b, int interval_ms) {
  this->blink_color = rgb_color{r, g, b};
  start_timer(interval_ms, [this]() {
    ESP_LOGI(TAG, "blinking...%d", this->blink_state);
    if (this->blink_state) {
      auto color = this->blink_color;
      led_strip_set_pixel(this->led_strip, 0, color.r, color.g, color.b);
      led_strip_refresh(this->led_strip);
    } else {
      led_strip_clear(this->led_strip);
    }
    this->blink_state = !this->blink_state;
  });
}

void rgb_led::breathe(int interval_ms) {
  start_timer(interval_ms, [this]() {
    float brightness = (sin(phase) + 1) * 127.5f;
    uint8_t r, g, b;
    fast_hsv2rgb_32bit(hue, saturation, (uint8_t)roundf(brightness), &r, &g,
                       &b);
    led_strip_set_pixel(this->led_strip, 0, r, g, b);
    led_strip_refresh(this->led_strip);

    // 更新色相和呼吸相位
    this->hue = (uint16_t)(this->hue + HUE_SPEED) % HSV_HUE_MAX;
    this->phase = fmod(this->phase + BREATHE_SPEED, 2 * PI);
  });
}

void rgb_led::turn_off() {}

void rgb_led::start_timer(int interval_ms, std::function<void()> callback) {
  if (this->led_strip == nullptr) {
    return;
  }
  std::lock_guard<std::mutex> lock(this->mutex);
  esp_timer_stop(this->strip_timer);
  this->strip_callback = callback;
  esp_timer_start_periodic(this->strip_timer, interval_ms * 1000);
  ESP_LOGI(TAG, "LED timer started");
}