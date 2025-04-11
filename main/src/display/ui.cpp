#include "display/ui.h"
#include "display/lv_font.h"
#include <lv_init.h>

using namespace walle;

static const char *TAG = "ui";

ui::ui(lv_display_t *display) : display(display) {}

void ui::initialize() {
  this->splash_screen = lv_obj_create(NULL);
  this->main_screen = lv_obj_create(NULL);

  this->create_splash_screen();
}

void ui::show_splash() { lv_scr_load(this->splash_screen); }

void ui::create_splash_screen() {
  this->main_label = lv_label_create(this->splash_screen);

  lv_label_set_text(main_label, "系统加载中...");

  lv_obj_set_style_text_font(main_label, &wqy_st_12, 0);
  lv_obj_set_style_text_align(main_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_center(main_label);
}