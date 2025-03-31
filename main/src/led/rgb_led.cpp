#include "led/rgb_led.h"
#include <esp_log.h>

#include "application.h"

using namespace walle;

#define TAG "RgbLed"

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
    show(20, 0, 0);
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

void rgb_led::blink(uint8_t r, uint8_t g, uint8_t b, int interval_ms) {}

void rgb_led::breathe() {}

void rgb_led::turn_off() {}

void rgb_led::start_timer(int interval_ms, std::function<void()> callback) {
  if (this->led_strip == nullptr) {
    return;
  }
  std::lock_guard<std::mutex> lock(this->mutex);
  esp_timer_stop(this->strip_timer);
  this->strip_callback = callback;
  esp_timer_start_periodic(this->strip_timer, interval_ms * 1000);
}