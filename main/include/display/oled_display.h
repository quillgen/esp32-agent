#ifndef _OLED_DISPLAY_H_
#define _OLED_DISPLAY_H_

#include <esp_lcd_panel_io.h>
#include <esp_lcd_panel_ops.h>
#include <esp_lcd_panel_ssd1306.h>
#include <esp_lcd_panel_vendor.h>
#include <esp_timer.h>
#include <freertos/event_groups.h>
#include <lvgl.h>
#include <lvgl_private.h>

#include <functional>

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

  void update_time();

private:
  void configure_i2c();
  void ssd1306_init();
  void flip_screen();
  void create_splash_screen();
  void create_main_screen();

  void test_display();

private:
  static void lvgl_flush_callback(lv_display_t *disp, const lv_area_t *area,
                                  uint8_t *px_map);
  static void updateClockCallback(lv_timer_t *timer);

private:
  i2c_master_bus_handle_t i2c_bus_;
  esp_lcd_panel_handle_t panel_handle_;
  esp_lcd_panel_io_handle_t io_handle_;

  uint8_t *lvgl_draw_buffer_;

  esp_timer_handle_t lvgl_tick_timer_;
  lv_timer_t *clock_timer;

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
  lv_obj_t *gear_label_;
  lv_obj_t *info_bar_;
  lv_obj_t *status_label_;
};
} // namespace agent
#endif