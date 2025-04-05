#include "application.h"
#include <esp_log.h>

#include "led/rgb_led.h"

using namespace walle;

#define TAG "🤖 APP"

application::application() {
  this->event_group = xEventGroupCreate();
  this->_led = new rgb_led((gpio_num_t)CONFIG_BLINK_GPIO);
  this->_network = new network(this->event_group);
}

application::~application() {
  if (this->_led != nullptr)
    delete this->_led;
  if (this->_network != nullptr)
    delete this->_network;
  vEventGroupDelete(this->event_group);
}

void application::start() {
  set_state(starting);
  this->_network->init_wifi();
}

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
