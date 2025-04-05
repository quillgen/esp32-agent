#include <math.h>
#include <stdio.h>
#include <string.h>

#include <driver/gpio.h>
#include <driver/i2c_master.h>
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

#define PIN_OLED_RST 11

#define DISPLAY_SDA_PIN 9
#define DISPLAY_SCL_PIN 10

// https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/lcd/spi_lcd.html
void configure_spi(void) {

  i2c_master_bus_handle_t i2c_bus;
  i2c_master_bus_config_t bus_cfg = {
      .i2c_port = (i2c_port_t)0,
      .sda_io_num = (gpio_num_t)DISPLAY_SDA_PIN,
      .scl_io_num = (gpio_num_t)DISPLAY_SCL_PIN,
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .glitch_ignore_cnt = 7,
      .intr_priority = 0,
      .trans_queue_depth = 0,
      .flags =
          {
              .enable_internal_pullup = 1,
          },
  };
  ESP_ERROR_CHECK(i2c_new_master_bus(&bus_cfg, &i2c_bus));

  esp_lcd_panel_io_i2c_config_t io_config = {
      .dev_addr = 0x78,
      .on_color_trans_done = nullptr,
      .user_ctx = nullptr,
      .control_phase_bytes = 1,
      .dc_bit_offset = 6,
      .lcd_cmd_bits = 8,
      .lcd_param_bits = 8,
      .flags =
          {
              .dc_low_on_data = 0,
              .disable_control_phase = 0,
          },
      .scl_speed_hz = 400 * 1000,
  };

  esp_lcd_panel_io_handle_t io_handle = NULL;
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c_v2(i2c_bus, &io_config, &io_handle));

  gpio_set_direction((gpio_num_t)PIN_OLED_RST, GPIO_MODE_OUTPUT);
  gpio_set_level((gpio_num_t)PIN_OLED_RST, 0);
  vTaskDelay(pdMS_TO_TICKS(200));
  gpio_set_level((gpio_num_t)PIN_OLED_RST, 1);
  vTaskDelay(pdMS_TO_TICKS(100));

  esp_lcd_panel_handle_t panel_handle = NULL;
  esp_lcd_panel_dev_config_t panel_config = {
      .reset_gpio_num = PIN_OLED_RST,
      //.bits_per_pixel = 1,
      .color_space = ESP_LCD_COLOR_SPACE_MONOCHROME,
      // .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
      .bits_per_pixel = 1,
  };
  // Create LCD panel handle for ST7789, with the SPI IO device handle
  ESP_ERROR_CHECK(
      esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

  uint8_t buffer[128 * 64 / 8] = {0};
  memset(buffer, 0xAA, sizeof(buffer)); // 全屏白色
  esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, 128, 64, buffer);

  ESP_LOGI(TAG, "Display initialized!");
}

extern "C" void app_main(void) {
  ESP_LOGI(TAG, "ESP32 agent running!");
  // configure_spi();
  //  //  application::instance().start();
  //  gpio_num_t pin = (gpio_num_t)12;
  //  // gpio_config_t io_conf = {
  //  //     .pin_bit_mask = (1ULL << 42),      // 选择 GPIO 42
  //  //     .mode = GPIO_MODE_OUTPUT,          // 设置为输出模式（或
  //  //     GPIO_MODE_INPUT .pull_up_en = GPIO_PULLUP_DISABLE, // 禁用上拉电阻
  //  //     .pull_down_en = GPIO_PULLDOWN_DISABLE, // 禁用下拉电阻
  //  //     .intr_type = GPIO_INTR_DISABLE         // 禁用中断
  //  // };

  // // // 应用配置
  // // gpio_config(&io_conf);
  // gpio_set_direction(pin, GPIO_MODE_OUTPUT); // Built-in LED on GPIO 0
  while (1) {
    // gpio_set_level(pin, 0);
    vTaskDelay(100);
    // gpio_set_level(pin, 1);
    // vTaskDelay(100);
  }
}
