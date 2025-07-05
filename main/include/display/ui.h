#ifndef __UI_H_
#define __UI_H_

#include <lvgl.h>

namespace agent {
class Ui {
public:
  Ui(lv_display_t *display);

public:
  void initialize();
  void show_splash();
  void set_network_connecting();
  void set_nstp();
  void show_main();
  void on_lv_tick();

private:
  void create_splash_screen();
  void create_main_screen();
  void update_time();

private:
  lv_display_t *display_;
  lv_obj_t *splash_screen_;
  lv_obj_t *progress_label_;
  lv_obj_t *welcome_;
  lv_obj_t *main_screen_;
  lv_obj_t *status_bar_;
  lv_obj_t *battery_label_;
  lv_obj_t *wifi_label_;
  lv_obj_t *bluetooth_label_;
  lv_obj_t *time_label_;
  lv_obj_t *main_label_;
};
} // namespace agent

#endif