#include "application.h"
#include <esp_log.h>
#include <esp_netif_sntp.h>
#include <esp_sntp.h>

#include "display/ssd1306_i2c.h"
#include "event.h"
#include "led/rgb_led.h"

using namespace agent;

static const int MAX_RETRY_COUNT = 3;

#define NTP_SERVER_1 "ntp.aliyun.com"
#define NTP_SERVER_2 "ntp.aliyun.com"

#define TAG "app"

Application::Application() {
  event_group_ = xEventGroupCreate();
  led_ = new RgbLed((gpio_num_t)CONFIG_BLINK_GPIO);
  network_ = new network(event_group_);
  speaker_ = new speaker();
  mic_ = new Mic();
  oled_ = new Ssd1306OledI2c(event_group_);
}

Application::~Application() {
  if (led_ != nullptr)
    delete led_;
  if (network_ != nullptr)
    delete network_;
  if (speaker_ != nullptr)
    delete speaker_;
  if (mic_ != nullptr)
    delete mic_;
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

  speaker_->init_speaker();
  mic_->init();
  set_state(AppState::kNetworkConnecting);
  network_->init_wifi();

  EventBits_t bits =
      xEventGroupWaitBits(event_group_, BIT_WIFI_CONNECTED | BIT_WIFI_FAILED,
                          pdFALSE, pdFALSE, portMAX_DELAY);
  if (bits & BIT_WIFI_CONNECTED) {
    ESP_LOGI(TAG, "connected to ap");
    set_state(AppState::kNstp);
    sync_time();
  } else if (bits & BIT_WIFI_FAILED) {
    ESP_LOGE(TAG, "Failed to connect to SSID:%s, password:%s",
             CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
    set_state(AppState::kNetworkNotConnected);
  } else {
    ESP_LOGE(TAG, "UNEXPECTED EVENT");
  }

  set_state(AppState::kIdle);
  // speaker_->test();
}

void time_sync_notification_cb(struct timeval *tv) {
  ESP_LOGI(TAG, "sntp time sync done!");
}

void Application::sync_time() {
  set_state(AppState::kNstp);
  esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG(NTP_SERVER_1);
  config.sync_cb = time_sync_notification_cb;
  esp_netif_sntp_init(&config);

  int retry = 0;
  const int max_retry = 15;
  while (esp_netif_sntp_sync_wait(2000 / portTICK_PERIOD_MS) ==
             ESP_ERR_TIMEOUT &&
         ++retry < max_retry) {
    ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry,
             max_retry);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  ESP_LOGI(TAG, "retry =%d", retry);

  time_t now;
  struct tm timeinfo;
  time(&now);
  setenv("TZ", "CST-8", 1);
  tzset();
  localtime_r(&now, &timeinfo);
  char strftime_buf[64];
  strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
  ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);
}

void Application::main_loop() {
  // while (1) {
  //   mic_->read_sound();
  // }
}

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
