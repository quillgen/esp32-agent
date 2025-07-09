#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>

#include "audio/mic.h"
#include "audio/speaker.h"
#include "display/oled_display.h"
#include "event.h"
#include "led/rgb_led.h"
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
  AppState get_state() const;

  void start();
  void reboot();

private:
  Application();
  ~Application();

private:
  static void main_task(void *arg);
  static void ui_task(void *arg);

  void handle_booting_state();
  void handle_network_connecting_state();
  void handle_idle_state();
  void handle_active_state();
  void handle_error_state();

  void sync_time();
  void set_state(AppState state);
  void update_display();

private:
  AppState state_;
  RgbLed *led_;
  OledDisplay *oled_;
  network *network_;
  speaker *speaker_;
  Mic *mic_;

private:
  EventGroupHandle_t event_group_;
  SemaphoreHandle_t state_mutex_;
  TaskHandle_t main_task_handle_;
  TaskHandle_t display_task_handle_;
};
} // namespace agent

#endif