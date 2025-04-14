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
  void show_main();
  void on_lv_tick();

private:
  void create_splash_screen();
  void create_main_screen();
  void update_time();

private:
  lv_display_t *display;
  lv_obj_t *splash_screen;
  lv_obj_t *progress_label;
  lv_obj_t *logo;
  lv_obj_t *logo_title;
  lv_obj_t *main_screen;
  lv_obj_t *status_bar;
  lv_obj_t *battery_label;
  lv_obj_t *wifi_label;
  lv_obj_t *time_label;
  lv_obj_t *main_label;
};
} // namespace agent

#endif