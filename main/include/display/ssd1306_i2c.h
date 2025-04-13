#ifndef __SSD1306_I2C_H_
#define __SSD1306_I2C_H_

#include "ssd1306_oled.h"

namespace agent {

class ssd1306_oled_i2c : public ssd1306_oled {
protected:
  void configure_io_bus() override;

private:
  void configure_i2c();

private:
  i2c_master_bus_handle_t i2c_bus = nullptr;
  esp_lcd_panel_handle_t panel_handle = nullptr;
};
} // namespace agent

#endif