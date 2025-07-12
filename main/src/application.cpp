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

#define TAG "application"

Application::Application()
    : state_(AppState::UNKNOWN), led_(nullptr), oled_(nullptr),
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

  // switch to booting when basic devices ready
  set_state(AppState::BOOTING);

  network_->init();
  speaker_->init_speaker();
  mic_->init();

  xTaskCreatePinnedToCore(main_task, "Main_Task", 8192, this, 2,
                          &main_task_handle_, APP_CPU_NUM);
  xTaskCreate(ui_task, "UI_Task", 8192, this, 1, &display_task_handle_);
  ui_queue = xQueueCreate(4, sizeof(UiStatus));

  if (ui_queue == NULL) {
    ESP_LOGE("UI", "Failed to create ui_queue!");
  }
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
    if (state_ != s) {
      state_ = s;
      xEventGroupSetBits(event_group_, Events::APP_STATE_CHANGED);
    }

    xSemaphoreGive(state_mutex_);
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
  const TickType_t xDelay = pdMS_TO_TICKS(500); // 500ms超时
  while (true) {
    EventBits_t bits = xEventGroupWaitBits(app->event_group_,
                                           Events::APP_STATE_CHANGED |
                                               Events::NETWORK_STATE_CHANGED,
                                           pdTRUE,  // 清除事件位
                                           pdFALSE, // 不等待所有位
                                           xDelay);
    if (bits & Events::APP_STATE_CHANGED) {
      AppState s = app->get_state();
      ESP_LOGI(TAG, "app state changed->%d", s);
      switch (s) {
      case AppState::BOOTING:
        app->handle_booting_state();
        break;
      default:
        break;
      }
    } else if (bits & Events::NETWORK_STATE_CHANGED) {
      NetworkState s = app->get_network_state();
      ESP_LOGI(TAG, "network state changed->%d", s);
      app->handle_network_state(s);
    }
  }
}

void Application::ui_task(void *arg) {
  Application *app = static_cast<Application *>(arg);

  // Watchdog: Register task if needed (ESP-IDF usually does this automatically
  // for FreeRTOS tasks)

  UiStatus ui_status;
  TickType_t last_wd_feed = xTaskGetTickCount();

  while (1) {
    // 1. Handle incoming UI messages
    // Use a non-blocking receive to allow LVGL to run smoothly
    while (xQueueReceive(app->ui_queue, &ui_status, 0) == pdTRUE) {
      // Update LVGL UI elements based on ui_status
      // (You should implement a method like app->update_ui(ui_status);)
      app->oled_->updateUi(ui_status);
    }

    // 2. Handle LVGL timers and screen refresh
    uint32_t time_till_next = lv_timer_handler();
    if (time_till_next == LV_NO_TIMER_READY) {
      time_till_next = LV_DEF_REFR_PERIOD;
    }

    // 3. Watchdog: Feed periodically (if you use esp_task_wdt)
    // Feed every 1s or as needed, depending on your watchdog configuration
    if ((xTaskGetTickCount() - last_wd_feed) > pdMS_TO_TICKS(1000)) {
      // esp_task_wdt_reset(); // Uncomment if you manually feed the watchdog
      last_wd_feed = xTaskGetTickCount();
    }

    // 4. Delay according to LVGL's needs, yield CPU if needed
    if (time_till_next > 0) {
      vTaskDelay(pdMS_TO_TICKS(time_till_next));
    } else {
      // Prevent tight loop, yield CPU
      taskYIELD();
    }
  }
}

void Application::handle_booting_state() {
  ESP_LOGI(TAG, "handling booting...");
  led_->blink(WARNING_COLOR.r, WARNING_COLOR.g, WARNING_COLOR.b, 500);
  UiStatus ui;
  ui.screen = Screen::Main;
  xQueueSend(ui_queue, &ui, portMAX_DELAY);
}

void Application::handle_network_state(NetworkState s) {
  UiStatus ui;
  ui.screen = Screen::Main;
  switch (s) {
  case NetworkState::DISCONNECTED:
    ui.wifi = WifiStatus::Off;
    break;
  case NetworkState::CONNECTING:
    ui.wifi = WifiStatus::Off;
    break;
  case NetworkState::CONNECTED:
    ui.wifi = WifiStatus::On;
    break;
  case NetworkState::NETWORK_ERROR:
    ui.wifi = WifiStatus::Off;
    ui.warning = WarningStatus::Warning;
    break;
  default:
    break;
  }
  ESP_LOGI(TAG, "==>%d, %d", s, ui.wifi);
  xQueueSend(ui_queue, &ui, portMAX_DELAY);
}

void Application::handle_idle_state() {
  // 等待用户输入或传感器事件
  // EventBits_t bits = xEventGroupWaitBits(
  //     event_group_, EVENT_USER_INPUT | EVENT_SENSOR_DATA_READY,
  //     pdTRUE,             // 清除事件位
  //     pdFALSE,            // 不等待所有位
  //     pdMS_TO_TICKS(1000) // 超时1秒
  // );

  // if (bits) {
  //   set_state(AppState::ACTIVE);
  // }
}

void Application::handle_active_state() {}
void Application::handle_error_state() {}

void Application::reboot() {
  ESP_LOGI(TAG, "rebooting the application...");
  esp_restart();
}
