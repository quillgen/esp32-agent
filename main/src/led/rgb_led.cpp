#include "led/rgb_led.h"
#include "application.h"
#include "led/fast_hsv2rgb.h"
#include <esp_log.h>
#include <math.h>
#include <stdio.h>

using namespace agent;

#define TAG "led"

const float PI = 3.14159265f;
const float HUE_SPEED = 3.0f;      // 色相变化速度（度/循环周期）
const float BREATHE_SPEED = 0.03f; // 呼吸速度（相位增量/循环周期）

RgbLed::RgbLed(gpio_num_t pin) : pin_(pin) {

  esp_timer_create_args_t strip_timer_args = {
      .callback =
          [](void *arg) {
            auto self = static_cast<RgbLed *>(arg);
            std::lock_guard<std::mutex> lock(self->mutex_);
            if (self->strip_callback_ != nullptr) {
              self->strip_callback_();
            }
          },
      .arg = this,
      .dispatch_method = ESP_TIMER_TASK,
      .name = "strip_timer",
      .skip_unhandled_events = false,
  };
  ESP_ERROR_CHECK(esp_timer_create(&strip_timer_args, &strip_timer_));
}

RgbLed::~RgbLed() {
  esp_timer_stop(strip_timer_);
  if (led_strip_ != nullptr) {
    led_strip_del(led_strip_);
  }
}

void RgbLed::init() {
  led_strip_config_t strip_config = {
      .strip_gpio_num = pin_,
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
      led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip_));
  led_strip_clear(led_strip_);
}

void RgbLed::show(uint8_t r, uint8_t g, uint8_t b) {
  std::lock_guard<std::mutex> lock(mutex_);
  ESP_LOGI(TAG, "set led to rgb mode");
  led_strip_set_pixel(led_strip_, 0, r, g, b);
  led_strip_refresh(led_strip_);
}

void RgbLed::blink(uint8_t r, uint8_t g, uint8_t b, int interval_ms) {
  blink_color_ = RgbColor{r, g, b};
  start_timer(interval_ms, [this]() {
    if (blink_state_) {
      auto color = blink_color_;
      led_strip_set_pixel(led_strip_, 0, color.r, color.g, color.b);
      led_strip_refresh(led_strip_);
    } else {
      led_strip_clear(led_strip_);
    }
    blink_state_ = !blink_state_;
  });
}

void RgbLed::breathe(int interval_ms) {
  start_timer(interval_ms, [this]() {
    float brightness = (sin(phase_) + 1) * 127.5f;
    uint8_t r, g, b;
    fast_hsv2rgb_32bit(hue_, saturation_, (uint8_t)roundf(brightness), &r, &g,
                       &b);
    led_strip_set_pixel(led_strip_, 0, r, g, b);
    led_strip_refresh(led_strip_);

    // 更新色相和呼吸相位
    hue_ = (uint16_t)(hue_ + HUE_SPEED) % HSV_HUE_MAX;
    phase_ = fmod(phase_ + BREATHE_SPEED, 2 * PI);
  });
}

void RgbLed::turn_off() {}

void RgbLed::clear() { esp_timer_stop(strip_timer_); }
void RgbLed::start_timer(int interval_ms, std::function<void()> callback) {
  if (led_strip_ == nullptr) {
    return;
  }
  std::lock_guard<std::mutex> lock(mutex_);
  esp_timer_stop(strip_timer_);
  strip_callback_ = callback;
  esp_timer_start_periodic(strip_timer_, interval_ms * 500);
  ESP_LOGI(TAG, "LED timer started");
}