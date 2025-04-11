#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>

#include "audio/speaker.h"
#include "display/ssd1306_oled.h"
#include "led/led.h"
#include "network.h"

namespace walle {

enum app_state {
  unknown,
  starting,
  configuring_wifi,
  sntp,
  idle,
  error,
};

class application {
public:
  static application &instance() {
    static application INSTANCE;
    return INSTANCE;
  }

public:
  application(const application &) = delete;
  application &operator=(const application &) = delete;

public:
  inline app_state get_state() { return this->state; }

  void start();
  void reboot();

private:
  application();
  ~application();

private:
  void main_loop();
  void set_state(app_state state);

private:
  volatile app_state state = unknown;
  led *_led = nullptr;
  network *_network = nullptr;
  speaker *_speaker = nullptr;
  ssd1306_oled *oled = nullptr;
  EventGroupHandle_t event_group = nullptr;
};
} // namespace walle

#endif