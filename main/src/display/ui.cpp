#include "display/ui.h"
#include "display/lv_font.h"
#include <ctime>
#include <lv_init.h>

using namespace agent;

static const char *TAG = "ui";

ui::ui(lv_display_t *display) : display(display) {}

void ui::initialize() {
  this->splash_screen = lv_obj_create(NULL);
  this->main_screen = lv_obj_create(NULL);

  this->create_splash_screen();
}

void ui::on_lv_tick() {
  static uint32_t last_update = 0;
  static uint32_t progress = 0;
  if (lv_tick_elaps(last_update) > 1000) {
    ++progress;
    uint8_t p = progress % 4;
    if (p == 0) {
      lv_label_set_text(progress_label, "系统加载中");
    } else if (p == 1) {
      lv_label_set_text(progress_label, "系统加载中.");
    } else if (p == 2) {
      lv_label_set_text(progress_label, "系统加载中..");
    } else {
      lv_label_set_text(progress_label, "系统加载中...");
    }
    last_update = lv_tick_get();
  }
}

void ui::update_time() {
  time_t now;
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);
  static char time_str[32];
  strftime(time_str, sizeof(time_str), "%H:%M:%S", &timeinfo);
}

void ui::show_splash() { lv_scr_load(this->splash_screen); }

void ui::create_splash_screen() {
  this->progress_label = lv_label_create(this->splash_screen);
  this->logo_title = lv_label_create(this->splash_screen);
  this->logo = lv_image_create(this->splash_screen);

  lv_label_set_text(logo_title, "快翎智能");
  lv_obj_set_style_text_font(logo_title, &wqy_st_12, 0);
  lv_obj_set_style_text_align(logo_title, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(logo_title, LV_ALIGN_BOTTOM_MID, 0, 0);

  lv_label_set_text(progress_label, "系统加载中...");
  lv_obj_set_style_text_font(progress_label, &wqy_st_12, 0);
  lv_obj_set_style_text_align(progress_label, LV_TEXT_ALIGN_CENTER, 0);

  lv_image_set_src(this->logo, &duby_logo);
  lv_obj_center(this->logo);
}