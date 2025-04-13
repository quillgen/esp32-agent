#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>

#include "audio/speaker.h"
#include "display/ssd1306_oled.h"
#include "event.h"
#include "led/led.h"
#include "mutex.h"
#include "network.h"

namespace agent {

enum AppState {
  unknown,
  starting,
  configuring_wifi,
  sntp,
  idle,
  error,
};

class Application {
public:
  static Application &instance() {
    static Application kInstance;
    return kInstance;
  }

public:
  Application(const Application &) = delete;
  Application &operator=(const Application &) = delete;

public:
  inline AppState get_state() { return this->state_; }

  void start();
  void reboot();

private:
  Application();
  ~Application();

private:
  void main_loop();
  void set_state(AppState state);

private:
  AppState state_ = unknown;
  Led *led_ = nullptr;
  network *network_ = nullptr;
  speaker *speaker_ = nullptr;
  ssd1306_oled *oled_ = nullptr;
  EventGroupHandle_t event_group_ = nullptr;
};
} // namespace agent

#endif