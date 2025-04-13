#ifndef __SSD1306_OLED_H_
#define __SSD1306_OLED_H_

#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_ssd1306.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_timer.h>
#include <lvgl.h>

#include "device.h"
#include "ui.h"
#include <functional>

namespace agent {
enum ssd1306_type {
  iic,
  spi,
};

class ssd1306_oled : public Device {
public:
  ssd1306_oled();
  virtual ~ssd1306_oled();

public:
  void init() override;
  void turn_on();
  void turn_off();
  void clear();

protected:
  virtual void configure_io_bus() = 0;
  esp_lcd_panel_io_handle_t io_handle = nullptr;

private:
  void init_oled();
  void flip_screen();
  void init_ui();
  void update_time();

private:
  esp_lcd_panel_handle_t panel_handle;
  lv_display_t *display;
  uint8_t *buffer = nullptr;
  esp_timer_handle_t lvgl_timer = nullptr;
  ui *_ui = nullptr;
};
} // namespace agent

#endif