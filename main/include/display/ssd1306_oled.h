#ifndef __SSD1306_OLED_H_
#define __SSD1306_OLED_H_

#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_ssd1306.h>
#include <esp_lcd_panel_vendor.h>

#include "device.h"

namespace walle {
enum ssd1306_type {
  iic,
  spi,
};

class ssd1306_oled : public device {
public:
  void init() override;
  void turn_on();
  void turn_off();

protected:
  virtual void configure_io_bus() = 0;
  esp_lcd_panel_io_handle_t io_handle = nullptr;

private:
  void init_oled();
  esp_lcd_panel_handle_t panel_handle;
};
} // namespace walle

#endif