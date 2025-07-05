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

Ui::Ui(lv_display_t *display) : display_(display) {}

static lv_style_t defaultStyle;
static lv_style_t iconStyle;
static lv_style_t monoStyle;

void Ui::initialize() {
  splash_screen_ = lv_obj_create(NULL);
  main_screen_ = lv_obj_create(NULL);

  lv_style_init(&defaultStyle);
  lv_style_set_text_font(&defaultStyle, &wqy_st_14);
  lv_style_init(&iconStyle);
  lv_style_set_text_font(&iconStyle, &md_icons_14);
  lv_style_init(&monoStyle);
  lv_style_set_text_font(&monoStyle, &inconsolata_14);
  create_splash_screen();
  create_main_screen();
}

void Ui::on_lv_tick() {

  static uint32_t last_update = 0;
  static uint32_t progress = 0;
  if (lv_tick_elaps(last_update) > 1000) {
    update_time();
    last_update = lv_tick_get();
  }
}

void Ui::update_time() {
  time_t now;
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);
  static char time_str[32];
  strftime(time_str, sizeof(time_str), "%H:%M", &timeinfo);

  lv_label_set_text(this->time_label_, time_str);
}

void Ui::show_splash() { lv_scr_load(splash_screen_); }

void Ui::show_main() { lv_scr_load(main_screen_); }

void Ui::create_splash_screen() {
  welcome_ = lv_label_create(splash_screen_);
  progress_label_ = lv_label_create(splash_screen_);
  lv_label_set_text(progress_label_, "");
  lv_label_set_text(welcome_, "Pico Pilot");
  lv_obj_center(welcome_);
  lv_obj_align(progress_label_, LV_ALIGN_TOP_LEFT, 0, 5);

  lv_obj_add_style(progress_label_, &defaultStyle, 0);
}

void Ui::set_network_connecting() {
  lv_label_set_text(progress_label_, "网络初始化中...");
}

void Ui::set_nstp() { lv_label_set_text(progress_label_, "正在同步时间..."); }

void Ui::create_main_screen() {
  // Create status bar container
  status_bar_ = lv_obj_create(main_screen_);
  lv_obj_set_size(status_bar_, OLED_WIDTH, STATUS_BAR_HEIGHT);
  lv_obj_align(status_bar_, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_set_style_pad_all(status_bar_, 0, 0);

  // 设置flex布局：水平排列，左对齐，垂直居中
  lv_obj_set_flex_flow(status_bar_, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(status_bar_, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);

  // Status bar elements
  battery_label_ = lv_label_create(status_bar_);
  wifi_label_ = lv_label_create(status_bar_);
  bluetooth_label_ = lv_label_create(status_bar_);
  lv_label_set_text(wifi_label_, MD_ICON_SINGAL_ON);
  lv_label_set_text(battery_label_, MD_ICON_BATTERY_75);
  lv_label_set_text(bluetooth_label_, MD_ICON_BLUETOOTH_ON);
  lv_obj_add_style(battery_label_, &iconStyle, 0);
  lv_obj_add_style(wifi_label_, &iconStyle, 0);
  lv_obj_add_style(bluetooth_label_, &iconStyle, 0);

  lv_obj_t *spacer = lv_obj_create(status_bar_);
  lv_obj_set_flex_grow(spacer, 1); // 关键：占据剩余空间
  lv_obj_set_style_bg_opa(spacer, LV_OPA_TRANSP, LV_PART_MAIN); // 透明
  lv_obj_set_style_border_width(spacer, 0, LV_PART_MAIN);

  time_label_ = lv_label_create(status_bar_);
  lv_label_set_text(time_label_, "12:08");
  lv_obj_add_style(time_label_, &monoStyle, 0);

  // Add main content
  main_label_ = lv_label_create(main_screen_);
  lv_label_set_text(main_label_,
                    "人类的悲欢并不相通，\n我只是觉得他们吵闹。\n——鲁迅");
  lv_obj_set_style_text_font(main_label_, &wqy_st_14, 0);
  lv_obj_set_style_text_align(main_label_, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_center(main_label_);
}