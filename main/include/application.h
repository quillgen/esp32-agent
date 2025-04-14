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
  void sync_time();
  void watch_state_changed();
  void main_loop();
  void set_state(AppState state);

private:
  AppState state_ = AppState::kUnknown;
  Led *led_ = nullptr;
  network *network_ = nullptr;
  speaker *speaker_ = nullptr;
  Ssd1306Oled *oled_ = nullptr;
  EventGroupHandle_t event_group_ = nullptr;
  Mutex mutex_;
};
} // namespace agent

#endif