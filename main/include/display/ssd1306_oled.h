#ifndef __SSD1306_OLED_H_
#define __SSD1306_OLED_H_

#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_ssd1306.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_timer.h>
#include <freertos/event_groups.h>
#include <lvgl.h>

#include "device.h"
#include "event.h"
#include "ui.h"
#include <functional>

namespace agent {
enum OledType {
  iic,
  spi,
};

class Ssd1306Oled : public Device {
public:
  Ssd1306Oled(EventGroupHandle_t e);
  virtual ~Ssd1306Oled();

public:
  void init() override;
  void on_state_changed(AppState s);
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
  EventGroupHandle_t event_group_;
  esp_lcd_panel_handle_t panel_handle_;
  lv_display_t *display_;
  uint8_t *buffer_ = nullptr;
  esp_timer_handle_t lvgl_timer_ = nullptr;
  Ui *ui_ = nullptr;
};
} // namespace agent

#endif