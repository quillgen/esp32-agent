#include "display/ssd1306_oled.h"
#include <string.h>

#include <driver/i2c_master.h>

using namespace walle;

static const char *TAG = "oled";

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

void ssd1306_oled::init() {
  this->configure_io_bus();
  this->init_oled();
}

void ssd1306_oled::init_oled() {
  assert(this->io_handle);
  esp_lcd_panel_dev_config_t panel_config = {
      .reset_gpio_num = -1,

      .bits_per_pixel = 1,
  };
  esp_lcd_panel_ssd1306_config_t ssd1306_config = {
      .height = static_cast<uint8_t>(OLED_HEIGHT),
  };
  panel_config.vendor_config = &ssd1306_config;

  ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(this->io_handle, &panel_config,
                                            &this->panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_reset(this->panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(this->panel_handle));

  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(this->panel_handle, true));

  uint8_t buffer[128 * 64 / 8] = {0};
  memset(buffer, 0xAA, sizeof(buffer)); // 全屏白色
  esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, 128, 64, buffer);
}