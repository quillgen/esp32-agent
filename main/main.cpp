#include <math.h>
#include <stdio.h>
#include <string.h>

#include <driver/gpio.h>
#include <driver/i2s.h>
#include <driver/spi_common.h>
#include <driver/spi_master.h>

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
#include "led/fast_hsv2rgb.h"
#include "sdkconfig.h"
#include "wav.h"

using namespace walle;

static const char *TAG = "example";

// https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/lcd/spi_lcd.html
void configure_spi(void) {
  // 初始化 SPI 总线
  spi_bus_config_t bus_cfg = {
      .mosi_io_num = 47,
      .miso_io_num = -1, // 无 MISO
      .sclk_io_num = 21,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = 4096,
  };
  ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_DISABLED));
  esp_lcd_panel_io_handle_t io_handle = NULL;
  esp_lcd_panel_io_spi_config_t io_config = {
      .cs_gpio_num = 41,
      .dc_gpio_num = 40,
      .spi_mode = 0,
      .pclk_hz = 40 * 1000 * 1000,
      .trans_queue_depth = 10,
      .lcd_cmd_bits = 8,
      .lcd_param_bits = 8,
  };
  // Attach the LCD to the SPI bus
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI2_HOST,
                                           &io_config, &io_handle));

  esp_lcd_panel_handle_t panel_handle = NULL;
  esp_lcd_panel_dev_config_t panel_config = {
      .reset_gpio_num = 45,
      .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
      .bits_per_pixel = 1,
  };
  // Create LCD panel handle for ST7789, with the SPI IO device handle
  ESP_ERROR_CHECK(
      esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

  uint8_t buffer[128 * 64 / 8] = {0};
  memset(buffer, 0xFF, sizeof(buffer)); // 全屏白色
  esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, 128, 64, buffer);

  ESP_LOGI(TAG, "Display initialized!");
}

extern "C" void app_main(void) {
  ESP_LOGI(TAG, "ESP32 agent running!");
  configure_spi();
  application::instance().start();
  int i = 0;
  while (1) {
    i += 1;
  }
}
