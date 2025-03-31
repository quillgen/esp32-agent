#include "application.h"
#include <esp_log.h>

#include "led/rgb_led.h"

using namespace walle;

#define TAG "Application"

application::application() {
  this->_led = new rgb_led((gpio_num_t)CONFIG_BLINK_GPIO);
}

application::~application() { delete this->_led; }

void application::start() { set_state(starting); }

void application::main_loop() {}

void application::set_state(app_state state) {
  if (this->state == state) {
    return;
  }
  this->state = state;
  ESP_LOGI(TAG, "STATE: %d", this->state);
  this->_led->on_state_changed();
}

void application::reboot() {
  ESP_LOGI(TAG, "Rebooting the application...");
  esp_restart();
}
