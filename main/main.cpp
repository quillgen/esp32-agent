#include <math.h>
#include <stdio.h>
#include <string.h>

#include <esp_log.h>

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

extern "C" void app_main(void) {
  ++boot_count;
  ESP_LOGI(TAG, "ESP32 agent running! boot_count: %d", boot_count);
  time_t now;
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);

  ESP_LOGI(TAG, "time: year(%d)", timeinfo.tm_year);

  init_flash();

  Application &app = Application::instance();
  app.start();

  while (1) {
    lv_timer_handler();
    vTaskDelay(pdMS_TO_TICKS(5));
    esp_task_wdt_reset();
  }
}
