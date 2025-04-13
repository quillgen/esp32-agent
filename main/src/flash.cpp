#include "flash.h"

#include <esp_log.h>
#include <nvs.h>
#include <nvs_flash.h>

static const char *TAG = "🤖 FLASH";

void agent::init_flash() {
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_LOGW(TAG, "Erasing NVS flash to fix corruption");
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
}