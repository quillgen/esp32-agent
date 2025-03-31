#ifndef __RGB_LED_H_
#define __RGB_LED_H_

#include <atomic>
#include <driver/gpio.h>
#include <esp_timer.h>
#include <led_strip.h>
#include <mutex>

#include "led.h"

namespace walle {
struct rgb_color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

class rgb_led : public led {
public:
  rgb_led(gpio_num_t pin);
  virtual ~rgb_led();

public:
  void on_state_changed() override;
  void show(uint8_t r, uint8_t g, uint8_t b);
  void blink(uint8_t r, uint8_t g, uint8_t b, int interval_ms);
  void breathe(int interval_ms);
  void turn_off();

private:
  void start_timer(int interval_ms, std::function<void()> callback);

private:
  gpio_num_t pin;
  led_strip_handle_t led_strip = nullptr;
  esp_timer_handle_t strip_timer = nullptr;
  std::mutex mutex;
  std::function<void()> strip_callback = nullptr;
  bool blink_state = false;
  rgb_color blink_color = {};
  uint16_t phase = 0;
  uint16_t hue = 0;
  uint8_t saturation = 255;
};
} // namespace walle

#endif