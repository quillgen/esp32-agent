#ifndef __SSD1306_I2C_H_
#define __SSD1306_I2C_H_

#include "ssd1306_oled.h"

namespace agent {

class Ssd1306OledI2c : public Ssd1306Oled {
public:
  Ssd1306OledI2c(EventGroupHandle_t e) : Ssd1306Oled(e) {}

protected:
  void configure_io_bus() override;

private:
  void configure_i2c();

private:
  i2c_master_bus_handle_t i2c_bus_ = nullptr;
};
} // namespace agent

#endif