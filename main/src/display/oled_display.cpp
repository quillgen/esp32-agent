#include "display/oled_display.h"

#include "display/lv_font.h"
#include <cstring>
#include <ctime>
#include <driver/i2c_master.h>
#include <esp_err.h>
#include <esp_log.h>

using namespace agent;

static const char *TAG = "oled";

#define OLED_WIDTH 128
#define OLED_HEIGHT 64

#define STATUS_BAR_HEIGHT (OLED_HEIGHT / 4)

#define LVGL_TICK_PERIOD_MS 5

static uint8_t oled_buffer[OLED_WIDTH * OLED_HEIGHT / 8];
// LVGL 9: must include palette size for 1-bit buffer
#define PALETTE_SIZE 8
size_t buffer_size = (OLED_WIDTH * OLED_HEIGHT) / 8 + PALETTE_SIZE;

static lv_style_t defaultStyle;
static lv_style_t iconStyle;
static lv_style_t monoStyle;

OledDisplay::OledDisplay()
    : i2c_bus_(nullptr), panel_handle_(nullptr), io_handle_(nullptr),
      display_(nullptr), splash_screen_(nullptr), progress_label_(nullptr),
      welcome_(nullptr), main_screen_(nullptr), status_bar_(nullptr),
      battery_label_(nullptr), wifi_label_(nullptr), bluetooth_label_(nullptr),
      mic_label_(nullptr), radar_label_(nullptr), time_label_(nullptr),
      gear_label_(nullptr) {
  lvgl_draw_buffer_ = new uint8_t[buffer_size];
}

OledDisplay::~OledDisplay() {
  if (display_ != nullptr) {
    lv_display_delete(display_);
  }
  if (lvgl_draw_buffer_)
    delete[] lvgl_draw_buffer_;
}

void OledDisplay::configure_i2c() {
  i2c_master_bus_config_t bus_cfg = {
      .i2c_port = (i2c_port_t)0,
      .sda_io_num = (gpio_num_t)OLED_I2C_SDA_PIN,
      .scl_io_num = (gpio_num_t)OLED_I2C_SCL_PIN,
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .glitch_ignore_cnt = 7,
      .intr_priority = 0,
      .trans_queue_depth = 0,
      .flags =
          {
              .enable_internal_pullup = 1,
          },
  };
  ESP_ERROR_CHECK(i2c_new_master_bus(&bus_cfg, &i2c_bus_));

  esp_lcd_panel_io_i2c_config_t io_config = {
      .dev_addr = 0x3C,
      .on_color_trans_done = nullptr,
      .user_ctx = nullptr,
      .control_phase_bytes = 1,
      .dc_bit_offset = 6,
      .lcd_cmd_bits = 8,
      .lcd_param_bits = 8,
      .flags =
          {
              .dc_low_on_data = 0,
              .disable_control_phase = 0,
          },
      .scl_speed_hz = 200 * 1000,
  };

  ESP_ERROR_CHECK(
      esp_lcd_new_panel_io_i2c_v2(i2c_bus_, &io_config, &io_handle_));
}

void OledDisplay::flip_screen() {
  /**
   * see: https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
   *  - 10.1.8  Set Segment Re-map (A0h/A1h)
   *  - 10.1.14 Set COM Output Scan Direction (C0h/C8h)
   *
   * or if the hardware does not support flipping, could use:
   * esp_lcd_panel_mirror(panel_handle, true, true);
   */

  esp_lcd_panel_io_tx_param(io_handle_, 0xA1, (uint8_t[]){0x01}, 1);
  esp_lcd_panel_io_tx_param(io_handle_, 0xC8, (uint8_t[]){0x08}, 1);
}

void OledDisplay::ssd1306_init() {
  esp_lcd_panel_dev_config_t panel_config = {
      .reset_gpio_num = -1,
      .bits_per_pixel = 1,
  };
  esp_lcd_panel_ssd1306_config_t ssd1306_config = {
      .height = static_cast<uint8_t>(OLED_HEIGHT),
  };
  panel_config.vendor_config = &ssd1306_config;

  ESP_ERROR_CHECK(
      esp_lcd_new_panel_ssd1306(io_handle_, &panel_config, &panel_handle_));
  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle_));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle_));

  flip_screen();
  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle_, true));
}

static bool notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t io_panel,
                                    esp_lcd_panel_io_event_data_t *edata,
                                    void *user_ctx) {
  lv_display_t *disp = (lv_display_t *)user_ctx;
  lv_display_flush_ready(disp);
  return false;
}

void OledDisplay::lvgl_flush_callback(lv_display_t *disp, const lv_area_t *area,
                                      uint8_t *px_map) {
  esp_lcd_panel_handle_t panel_handle =
      static_cast<esp_lcd_panel_handle_t>(lv_display_get_user_data(disp));
  const int hor_res = lv_display_get_physical_horizontal_resolution(disp);

  // Skip palette bytes
  px_map += PALETTE_SIZE;

  int x1 = area->x1, x2 = area->x2;
  int y1 = area->y1, y2 = area->y2;

  // For each column and SSD1306 page (8 rows)
  for (int col = x1; col <= x2; ++col) {
    for (int page = y1 / 8; page <= y2 / 8; ++page) {
      uint8_t page_byte = 0;
      for (int bit = 0; bit < 8; ++bit) {
        int y = page * 8 + bit;
        if (y < y1 || y > y2)
          continue;

        // LVGL buffer: (y * hor_res + col) => bit in the buffer
        size_t px_offset = y * hor_res + col;
        size_t byte_offset = px_offset / 8;
        size_t bit_offset = 7 - (px_offset % 8);

        // LVGL: 1 = black (background), 0 = white (foreground)
        // SSD1306: 1 = pixel ON, 0 = pixel OFF
        // You might need to invert here depending on your desired color theme
        bool pixel_on = ((px_map[byte_offset] >> bit_offset) & 0x1) ==
                        0; // Invert for white foreground

        if (pixel_on)
          page_byte |= (1 << bit);
      }
      oled_buffer[page * OLED_WIDTH + col] = page_byte;
    }
  }

  esp_lcd_panel_draw_bitmap(panel_handle, x1, y1, x2 + 1, y2 + 1, oled_buffer);
  lv_display_flush_ready(disp);
}

void OledDisplay::updateClockCallback(lv_timer_t *timer) {
  OledDisplay *self = static_cast<OledDisplay *>(timer->user_data);
  if (!self)
    return;

  self->update_time();
}

void OledDisplay::init() {
  lv_init();

  configure_i2c();
  ssd1306_init();

  display_ = lv_display_create(OLED_WIDTH, OLED_HEIGHT);
  lv_display_set_user_data(display_, panel_handle_);
  lv_display_set_color_format(display_, LV_COLOR_FORMAT_I1);
  /**
   * TODO: does not work with LV_DISPLAY_RENDER_MODE_PARTIAL
   * For partial mode, LVGL expects the buffer to be at least as big as the
   * largest area you’ll flush (e.g., a whole row or column).
   */
  lv_display_set_buffers(display_, lvgl_draw_buffer_, NULL, buffer_size,
                         LV_DISPLAY_RENDER_MODE_FULL);
  lv_display_set_flush_cb(display_, lvgl_flush_callback);

  const esp_lcd_panel_io_callbacks_t cbs = {
      .on_color_trans_done = notify_lvgl_flush_ready,
  };
  /* Register done callback */
  esp_lcd_panel_io_register_event_callbacks(io_handle_, &cbs, display_);

  esp_timer_create_args_t timer_args = {
      .callback = [](void *arg) { lv_tick_inc(LVGL_TICK_PERIOD_MS); },
      .dispatch_method = ESP_TIMER_TASK,
      .name = "lvgl_tick",
      .skip_unhandled_events = false,
  };
  esp_timer_create(&timer_args, &lvgl_tick_timer_);
  esp_timer_start_periodic(lvgl_tick_timer_, LVGL_TICK_PERIOD_MS * 1000);

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

  // test_display();
  // vTaskDelay(pdMS_TO_TICKS(2000));

  lv_scr_load(splash_screen_);

  clock_timer = lv_timer_create(&OledDisplay::updateClockCallback, 1000, this);
}

void OledDisplay::updateUi(UiStatus i) {
  ESP_LOGI(TAG, "=> %d", i.wifi);
  if (i.screen == Screen::Splash) {
    lv_scr_load(splash_screen_);
  } else if (i.screen == Screen::Main) {
    lv_scr_load(main_screen_);
  }
  setWifiStatus(i.wifi);
}

void OledDisplay::create_splash_screen() {
  welcome_ = lv_label_create(splash_screen_);
  progress_label_ = lv_label_create(splash_screen_);
  lv_label_set_text(progress_label_, "");
  lv_label_set_text(welcome_, "Pico Pilot");
  lv_obj_center(welcome_);
  lv_obj_align(progress_label_, LV_ALIGN_TOP_LEFT, 0, 5);

  lv_obj_add_style(progress_label_, &defaultStyle, 0);
}

void OledDisplay::create_main_screen() {
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
  mic_label_ = lv_label_create(status_bar_);
  radar_label_ = lv_label_create(status_bar_);
  setWifiStatus(WifiStatus::Off);
  lv_label_set_text(battery_label_, MD_ICON_BATTERY_75);
  lv_label_set_text(bluetooth_label_, MD_ICON_BLUETOOTH_ON);
  lv_label_set_text(mic_label_, MD_ICON_MIC);
  lv_label_set_text(radar_label_, MD_ICON_RADAR);
  lv_obj_add_style(battery_label_, &iconStyle, 0);
  lv_obj_add_style(wifi_label_, &iconStyle, 0);
  lv_obj_add_style(bluetooth_label_, &iconStyle, 0);
  lv_obj_add_style(mic_label_, &iconStyle, 0);
  lv_obj_add_style(radar_label_, &iconStyle, 0);

  gear_label_ = lv_label_create(main_screen_);
  lv_label_set_text(gear_label_, "P");
  lv_obj_set_style_text_font(gear_label_, &inter_32, 0);
  lv_obj_set_style_text_align(gear_label_, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_center(gear_label_);

  info_bar_ = lv_obj_create(main_screen_);
  lv_obj_set_size(info_bar_, OLED_WIDTH, STATUS_BAR_HEIGHT);
  lv_obj_align(info_bar_, LV_ALIGN_TOP_LEFT, 0, 48);
  lv_obj_set_style_pad_all(info_bar_, 0, 0);
  lv_obj_set_flex_flow(info_bar_, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(info_bar_, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);

  time_label_ = lv_label_create(info_bar_);
  lv_label_set_text(time_label_, "12:08:05");
  lv_obj_add_style(time_label_, &monoStyle, 0);

  lv_obj_t *spacer = lv_obj_create(info_bar_);
  lv_obj_set_flex_grow(spacer, 1);
  lv_obj_set_style_bg_opa(spacer, LV_OPA_TRANSP, LV_PART_MAIN);
  lv_obj_set_style_border_width(spacer, 0, LV_PART_MAIN);

  status_label_ = lv_label_create(info_bar_);
  lv_label_set_text(status_label_, "就绪");
  lv_obj_add_style(status_label_, &defaultStyle, 0);
}

void OledDisplay::setWifiStatus(WifiStatus s) {
  ESP_LOGI(TAG, "updating wifi status->%d", s);
  if (s == WifiStatus::Off) {
    lv_label_set_text(wifi_label_, MD_ICON_SINGAL_OFF);
  } else {
    lv_label_set_text(wifi_label_, MD_ICON_SINGAL_ON);
  }
}
void OledDisplay::update_time() {
  time_t now;
  struct tm timeinfo;
  time(&now);
  localtime_r(&now, &timeinfo);
  static char time_str[32];
  strftime(time_str, sizeof(time_str), "%H:%M:%S", &timeinfo);

  lv_label_set_text(this->time_label_, time_str);
}

void OledDisplay::test_display() {
  // for (int page = 0; page < PAGES; page++) {
  //   for (int x = 0; x < OLED_WIDTH; x++) {
  //     // 每4像素一个条纹
  //     if (x % 8 < 4) {
  //       oled_buffer[page * OLED_WIDTH + x] = 0xFF; // 全亮
  //     } else {
  //       oled_buffer[page * OLED_WIDTH + x] = 0x00; // 全暗
  //     }
  //   }
  // }

  // esp_lcd_panel_draw_bitmap(panel_handle_, 0, 0, OLED_WIDTH, OLED_HEIGHT,
  //                           oled_buffer);
}