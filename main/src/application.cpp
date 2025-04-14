#include "application.h"
#include <esp_log.h>

#include "display/ssd1306_i2c.h"
#include "event.h"
#include "led/rgb_led.h"

using namespace agent;

#define TAG "app"

Application::Application() {
  event_group_ = xEventGroupCreate();
  led_ = new RgbLed((gpio_num_t)CONFIG_BLINK_GPIO);
  network_ = new network(event_group_);
  speaker_ = new speaker();
  oled_ = new Ssd1306OledI2c(event_group_);
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
  led_->init();
  oled_->init();

  // ensure led and oled init first
  xTaskCreate(
      [](void *arg) {
        Application *self = static_cast<Application *>(arg);
        self->watch_state_changed();
      },
      "LVGL", 4096, this, 0, NULL);
  set_state(AppState::kWifiProvisioning);
  network_->init_wifi();
  speaker_->init_speaker();
  set_state(AppState::kIdle);
  // speaker_->test();
}

void Application::main_loop() {}

void Application::set_state(AppState s) {
  if (state_ == s) {
    return;
  }
  if (mutex_.lock(pdMS_TO_TICKS(100))) {
    state_ = s;
    ESP_LOGI(TAG, "changed STATE: %d", state_);
    xEventGroupSetBits(event_group_, BIT_APP_STATE_CHANGED);
    mutex_.unlock();
  }
}

void Application::watch_state_changed() {
  while (1) {
    // Wait for the APP_STATE_CHANGED_BIT event
    EventBits_t uxBits = xEventGroupWaitBits(
        event_group_, BIT_APP_STATE_CHANGED,
        pdTRUE,  // Clear the bit after reading
        pdFALSE, // Wait for any bit (set to pdTRUE for "all bits")
        portMAX_DELAY);

    if (uxBits & BIT_APP_STATE_CHANGED) {
      // Read the shared state safely
      if (mutex_.lock(pdMS_TO_TICKS(100))) {
        AppState s = state_;
        ESP_LOGI(TAG, "app state changed to %d", s);
        led_->on_state_changed(s);
        oled_->on_state_changed(s);
        mutex_.unlock();
      }
    }
  }
}

void Application::reboot() {
  ESP_LOGI(TAG, "rebooting the application...");
  esp_restart();
}
