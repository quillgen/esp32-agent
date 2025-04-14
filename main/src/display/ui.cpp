#include "display/ui.h"
#include "display/lv_font.h"
#include <ctime>
#include <lv_init.h>

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define PALETTE_SIZE 8

#define STATUS_BAR_HEIGHT (OLED_HEIGHT / 4)

using namespace agent;

static const char *TAG = "ui";

Ui::Ui(lv_display_t *display) : display(display) {}

void Ui::initialize() {
  this->splash_screen = lv_obj_create(NULL);
  this->main_screen = lv_obj_create(NULL);

  this->create_splash_screen();
  this->create_main_screen();
}

void Ui::on_lv_tick() {
  static uint32_t last_update = 0;
  static uint32_t progress = 0;
  if (lv_tick_elaps(last_update) > 1000) {
    ++progress;
    uint8_t p = progress % 4;

    last_update = lv_tick_get();
  }
}

void Ui::update_time() {
  time_t now;
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);
  static char time_str[32];
  strftime(time_str, sizeof(time_str), "%H:%M:%S", &timeinfo);
}

void Ui::show_splash() { lv_scr_load(this->splash_screen); }

void Ui::show_main() { lv_scr_load(this->main_screen); }

void Ui::create_splash_screen() {
  this->logo = lv_image_create(this->splash_screen);
  lv_image_set_src(this->logo, &duby_logo);
  lv_obj_center(this->logo);
}

void Ui::create_main_screen() {
  // Create status bar container
  this->status_bar = lv_obj_create(main_screen);
  lv_obj_set_size(status_bar, OLED_WIDTH, STATUS_BAR_HEIGHT);
  lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);

  // Status bar elements
  this->battery_label = lv_label_create(status_bar);
  lv_label_set_text(battery_label, LV_SYMBOL_BATTERY_FULL);
  lv_obj_align(battery_label, LV_ALIGN_LEFT_MID, 0, 0);

  this->wifi_label = lv_label_create(status_bar);
  lv_label_set_text(wifi_label, LV_SYMBOL_WIFI);
  lv_obj_align(wifi_label, LV_ALIGN_LEFT_MID, 20, 0);

  this->time_label = lv_label_create(status_bar);
  lv_label_set_text(time_label, "00:00:00");
  lv_obj_align(time_label, LV_ALIGN_RIGHT_MID, 0, 0);

  // Add main content
  this->main_label = lv_label_create(main_screen);
  lv_label_set_text(main_label,
                    "人类的悲欢并不相通，\n我只是觉得他们吵闹。\n——鲁迅");
  lv_obj_set_style_text_font(main_label, &wqy_st_12, 0);
  lv_obj_set_style_text_align(main_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_center(main_label);
}