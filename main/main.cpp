#include <math.h>
#include <stdio.h>
#include <string.h>

#include <driver/gpio.h>
#include <driver/i2c_master.h>
#include <driver/i2s.h>
#include <driver/spi_common.h>
#include <driver/spi_master.h>

#include <esp_event.h>
#include <esp_lcd_io_spi.h>
#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_ssd1306.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <led_strip.h>
#include <lvgl.h>

#include "application.h"
#include "flash.h"
#include "led/fast_hsv2rgb.h"
#include "sdkconfig.h"
#include "wav.h"

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
