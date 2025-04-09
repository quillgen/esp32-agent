#include "application.h"
#include <esp_log.h>

#include "display/ssd1306_i2c.h"
#include "led/rgb_led.h"

using namespace walle;

#define TAG "🤖 APP"

application::application() {
  this->event_group = xEventGroupCreate();
  this->_led = new rgb_led((gpio_num_t)CONFIG_BLINK_GPIO);
  this->_network = new network(this->event_group);
  this->_speaker = new speaker();
  this->oled = new ssd1306_oled_i2c();
}

application::~application() {
  if (this->_led != nullptr)
    delete this->_led;
  if (this->_network != nullptr)
    delete this->_network;
  if (this->_speaker != nullptr)
    delete this->_speaker;
  if (this->oled != nullptr) {
    delete this->oled;
  }
  vEventGroupDelete(this->event_group);
}

void application::start() {
  this->oled->init();
  set_state(starting);
  this->_network->init_wifi();
  // this->_speaker->init_speaker();
  //  this->_speaker->test();
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
