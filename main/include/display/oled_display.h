#ifndef _OLED_DISPLAY_H_
#define _OLED_DISPLAY_H_

#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_ssd1306.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_timer.h>
#include <freertos/event_groups.h>
#include <lvgl.h>

#include <functional>

#include <lvgl.h>

#include "device.h"
#include "event.h"

namespace agent {
class OledDisplay : public Device {
public:
  OledDisplay();
  virtual ~OledDisplay();

public:
  void init() override;
  void refresh();

  void show_splash_screen();
  void show_network_status(bool connected);
  void show_idle_screen();
  void show_active_screen();
  void show_error_screen(const char *error);

private:
  void configure_i2c();
  void ssd1306_init();
  void flip_screen();
  void create_splash_screen();
  void create_main_screen();
  void update_time();
  void test_display();

private:
  i2c_master_bus_handle_t i2c_bus_;
  esp_lcd_panel_handle_t panel_handle_;
  esp_lcd_panel_io_handle_t io_handle_;

  uint8_t *buffer_ = nullptr;

  lv_display_t *display_;
  lv_obj_t *splash_screen_;
  lv_obj_t *progress_label_;
  lv_obj_t *welcome_;
  lv_obj_t *main_screen_;
  lv_obj_t *status_bar_;
  lv_obj_t *battery_label_;
  lv_obj_t *wifi_label_;
  lv_obj_t *bluetooth_label_;
  lv_obj_t *mic_label_;
  lv_obj_t *radar_label_;
  lv_obj_t *time_label_;
  lv_obj_t *main_label_;
  lv_obj_t *info_bar_;
};
} // namespace agent
#endif