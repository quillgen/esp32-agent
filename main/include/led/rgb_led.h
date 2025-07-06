#ifndef __RGB_LED_H_
#define __RGB_LED_H_

#include <atomic>
#include <driver/gpio.h>
#include <esp_timer.h>
#include <led_strip.h>
#include <mutex>

#include "led.h"

namespace agent {
struct RgbColor {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

class RgbLed : public Led {
public:
  RgbLed(gpio_num_t pin);
  virtual ~RgbLed();

public:
  void init() override;
  void show(uint8_t r, uint8_t g, uint8_t b);
  void blink(uint8_t r, uint8_t g, uint8_t b, int interval_ms);
  void breathe(int interval_ms);
  void turn_off();

private:
  void start_timer(int interval_ms, std::function<void()> callback);
  void clear();

private:
  gpio_num_t pin_;
  led_strip_handle_t led_strip_ = nullptr;
  esp_timer_handle_t strip_timer_ = nullptr;
  std::mutex mutex_;
  std::function<void()> strip_callback_ = nullptr;
  bool blink_state_ = false;
  RgbColor blink_color_ = {};
  uint16_t phase_ = 0;
  uint16_t hue_ = 0;
  uint8_t saturation_ = 255;
};
} // namespace agent

#endif