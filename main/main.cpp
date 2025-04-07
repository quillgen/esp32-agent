#include <math.h>
#include <stdio.h>
#include <string.h>

#include <esp_log.h>

#include "application.h"
#include "flash.h"

using namespace walle;

static const char *TAG = "🤖 MAIN";

extern "C" void app_main(void) {
  ESP_LOGI(TAG, "ESP32 agent running!");
  init_flash();

  application::instance().start();

  while (1) {
    vTaskDelay(100);
  }
}
