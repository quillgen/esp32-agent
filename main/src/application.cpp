#include "application.h"
#include <esp_log.h>
#include <esp_netif_sntp.h>
#include <esp_sntp.h>
#include <esp_task_wdt.h>

#include "event.h"
#include "led/rgb_led.h"

using namespace agent;

static const int MAX_RETRY_COUNT = 3;

const RgbColor ERROR_COLOR = RgbColor(10, 0, 0);
const RgbColor READY_COLOR = RgbColor(0, 10, 0);
const RgbColor WARNING_COLOR = RgbColor(0, 0, 10);

#define NTP_SERVER_1 "ntp.aliyun.com"
#define NTP_SERVER_2 "ntp.aliyun.com"

#define TAG "app"

Application::Application()
    : state_(AppState::BOOTING), led_(nullptr), oled_(nullptr),
      network_(nullptr), speaker_(nullptr), mic_(nullptr) {
  event_group_ = xEventGroupCreate();
  state_mutex_ = xSemaphoreCreateMutex();
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
  led_ = new RgbLed((gpio_num_t)CONFIG_BLINK_GPIO);
  network_ = new network(event_group_);
  speaker_ = new speaker();
  mic_ = new Mic();
  oled_ = new OledDisplay();

  led_->init();
  oled_->init();
  speaker_->init_speaker();
  mic_->init();
  network_->init_wifi();

  xTaskCreatePinnedToCore(main_task, "MainTask", 8192, this,
                          2, // 优先级 (高于其他任务)
                          &main_task_handle_, APP_CPU_NUM);
  xTaskCreate(display_task, "Display", 6144, this,
              1, // 中等优先级
              &display_task_handle_);

  // EventBits_t bits =
  //     xEventGroupWaitBits(event_group_, BIT_WIFI_CONNECTED | BIT_WIFI_FAILED,
  //                         pdFALSE, pdFALSE, portMAX_DELAY);
  // if (bits & BIT_WIFI_CONNECTED) {
  //   ESP_LOGI(TAG, "connected to ap");
  //   set_state(AppState::kNstp);
  //   // sync_time();
  // } else if (bits & BIT_WIFI_FAILED) {
  //   ESP_LOGE(TAG, "Failed to connect to SSID:%s, password:%s",
  //            CONFIG_ESP_WIFI_SSID, CONFIG_ESP_WIFI_PASSWORD);
  //   set_state(AppState::kNetworkNotConnected);
  // } else {
  //   ESP_LOGE(TAG, "UNEXPECTED EVENT");
  // }

  // set_state(AppState::kIdle);
  // speaker_->test();
}

void time_sync_notification_cb(struct timeval *tv) {
  ESP_LOGI(TAG, "sntp time sync done!");
}

void Application::sync_time() {
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

void Application::set_state(AppState s) {
  if (xSemaphoreTake(state_mutex_, portMAX_DELAY) == pdTRUE) {
    state_ = s;
    xSemaphoreGive(state_mutex_);

    xEventGroupSetBits(event_group_, EVENT_STATE_CHANGED);
  }
}

AppState Application::get_state() const {
  AppState state = AppState::UNKNOWN;
  if (xSemaphoreTake(state_mutex_, portMAX_DELAY) == pdTRUE) {
    state = state_;
    xSemaphoreGive(state_mutex_);
  }
  return state;
}

void Application::main_task(void *arg) {
  Application *app = static_cast<Application *>(arg);
  esp_task_wdt_add(NULL);

  while (true) {
    switch (app->get_state()) {
    case AppState::BOOTING:
      app->handle_booting_state();
      break;
    case AppState::NETWORK_CONNECTING:
      app->handle_network_connecting_state();
      break;
    case AppState::IDLE:
      app->handle_idle_state();
      break;
    case AppState::ACTIVE:
      app->handle_active_state();
      break;
    case AppState::ERROR:
      app->handle_error_state();
      break;
    default:
      break;
    }
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

void Application::display_task(void *arg) {
  Application *app = static_cast<Application *>(arg);
  while (true) {
    EventBits_t bits =
        xEventGroupWaitBits(app->event_group_, EVENT_STATE_CHANGED,
                            pdTRUE,  // 清除事件位
                            pdFALSE, // 不等待所有位
                            portMAX_DELAY);

    if (bits & EVENT_STATE_CHANGED) {
      app->update_display();
    }
    app->oled_->refresh();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void Application::handle_booting_state() {
  oled_->show_splash_screen();
  // network_->connect();
  set_state(AppState::NETWORK_CONNECTING);
}

void Application::handle_network_connecting_state() {
  // if (network_->is_connected()) {
  //   set_state(AppState::IDLE);
  // } else if (network_->connection_failed()) {
  //   set_state(AppState::ERROR);
  // }

  // oled_->show_network_status(network_->status());
}

void Application::handle_idle_state() {
  // 等待用户输入或传感器事件
  EventBits_t bits = xEventGroupWaitBits(
      event_group_, EVENT_USER_INPUT | EVENT_SENSOR_DATA_READY,
      pdTRUE,             // 清除事件位
      pdFALSE,            // 不等待所有位
      pdMS_TO_TICKS(1000) // 超时1秒
  );

  if (bits) {
    set_state(AppState::ACTIVE);
  }
}

void Application::handle_active_state() {}
void Application::handle_error_state() {}

void Application::update_display() {
  switch (get_state()) {
  case AppState::BOOTING:
    led_->blink(WARNING_COLOR.r, WARNING_COLOR.g, WARNING_COLOR.b, 500);
    break;
  case AppState::IDLE:
    led_->show(READY_COLOR.r, READY_COLOR.g, READY_COLOR.b);
    oled_->show_idle_screen();
    break;
  case AppState::ACTIVE:
    // oled_->show_active_screen(sensor_->latest_data());
    break;
  case AppState::ERROR:
    // oled_->show_error_screen(network_->last_error());
    break;
  default:
    break;
  }
}

void Application::reboot() {
  ESP_LOGI(TAG, "rebooting the application...");
  esp_restart();
}
