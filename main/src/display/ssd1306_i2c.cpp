#include "display/ssd1306_i2c.h"

#include <driver/i2c_master.h>

using namespace walle;

static const char *TAG = "oled";

#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 15

void ssd1306_oled_i2c::configure_i2c() {
  i2c_master_bus_config_t bus_cfg = {
      .i2c_port = (i2c_port_t)0,
      .sda_io_num = (gpio_num_t)I2C_SDA_PIN,
      .scl_io_num = (gpio_num_t)I2C_SCL_PIN,
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .glitch_ignore_cnt = 7,
      .intr_priority = 0,
      .trans_queue_depth = 0,
      .flags =
          {
              .enable_internal_pullup = 1,
          },
  };
  ESP_ERROR_CHECK(i2c_new_master_bus(&bus_cfg, &this->i2c_bus));
}

void ssd1306_oled_i2c::configure_io_bus() {
  this->configure_i2c();

  esp_lcd_panel_io_i2c_config_t io_config = {
      .dev_addr = 0x3C,
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

  ESP_ERROR_CHECK(
      esp_lcd_new_panel_io_i2c_v2(this->i2c_bus, &io_config, &this->io_handle));
}