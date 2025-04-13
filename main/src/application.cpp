#include "application.h"
#include <esp_log.h>

#include "display/ssd1306_i2c.h"
#include "led/rgb_led.h"

using namespace agent;

#define TAG "app"

Application::Application() {
  event_group_ = xEventGroupCreate();
  led_ = new RgbLed((gpio_num_t)CONFIG_BLINK_GPIO);
  network_ = new network(event_group_);
  speaker_ = new speaker();
  oled_ = new ssd1306_oled_i2c();
}

Application::~Application() {
  if (led_ != nullptr)
    delete led_;
  if (network_ != nullptr)
    delete network_;
  if (speaker_ != nullptr)
    delete speaker_;
  if (oled_ != nullptr) {
    delete oled_;
  }
  vEventGroupDelete(event_group_);
}

void Application::start() {
  set_state(starting);
  led_->init();
  oled_->init();
  network_->init_wifi();
  speaker_->init_speaker();
  // speaker_->test();
}

void Application::main_loop() {}

void Application::set_state(AppState s) {

  if (state_ == s) {
    return;
  }
  state_ = s;
  ESP_LOGI(TAG, "changed STATE: %d", state_);
  led_->on_state_changed();
}

void Application::reboot() {
  ESP_LOGI(TAG, "rebooting the application...");
  esp_restart();
}
