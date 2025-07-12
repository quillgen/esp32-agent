#include <math.h>
#include <stdio.h>
#include <string.h>

#include <esp_log.h>
#include <esp_psram.h>

#include "application.h"
#include "flash.h"
#include <esp_task_wdt.h>

using namespace agent;

static const char *TAG = "𓃰 main";

/* Variable holding number of times ESP32 restarted since first boot.
 * It is placed into RTC memory using RTC_DATA_ATTR and
 * maintains its value when ESP32 wakes from deep sleep.
 */
RTC_DATA_ATTR static int boot_count = 0;

void check_psram() {
  // 检查PSRAM初始化状态
  if (esp_psram_is_initialized()) {
    size_t psram_size = esp_psram_get_size();
    ESP_LOGI("PSRAM", "Detected %dMB PSRAM", psram_size / (1024 * 1024));
  } else {
    ESP_LOGW("PSRAM", "No PSRAM detected");
  }
}

extern "C" void app_main(void) {
  ++boot_count;
  ESP_LOGI(TAG, "ESP32 agent running! boot_count: %d", boot_count);

  time_t now;
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);

  ESP_LOGI(TAG, "time: year(%d)", timeinfo.tm_year);

  init_flash();
  mountSpiffs();

  check_psram();

  Application &app = Application::instance();
  app.start();

  // 注意：app_main函数不能返回，所以我们让这个任务挂起自己
  vTaskSuspend(NULL);
}
