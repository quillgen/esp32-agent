#include "display/oled_display.h"

#include <driver/i2c_master.h>
#include <esp_err.h>
#include <lvgl.h>

using namespace agent;

static const char *TAG = "oled";

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define PALETTE_SIZE 8

#define LVGL_TICK_PERIOD_MS 5

size_t buffer_size = (OLED_WIDTH * OLED_HEIGHT) / 8 + PALETTE_SIZE;
static uint8_t oled_buffer[OLED_WIDTH * OLED_HEIGHT / 8];

OledDisplay::OledDisplay()
    : i2c_bus_(nullptr), panel_handle_(nullptr), io_handle_(nullptr),
      display_(nullptr) {
  buffer_ = new uint8_t[buffer_size]{};
}

OledDisplay::~OledDisplay() {
  if (display_ != nullptr) {
    lv_display_delete(display_);
  }

  delete[] buffer_;
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

static void lvgl_flush_callback(lv_display_t *disp, const lv_area_t *area,
                                uint8_t *px_map) {
  esp_lcd_panel_handle_t panel_handle =
      static_cast<esp_lcd_panel_handle_t>(lv_display_get_user_data(disp));
  px_map += PALETTE_SIZE;

  uint16_t hor_res = lv_display_get_physical_horizontal_resolution(disp);
  int x1 = area->x1;
  int x2 = area->x2;
  int y1 = area->y1;
  int y2 = area->y2;

  // ESP_LOGI(TAG, "draw at: (%d, %d) (%d, %d)", x1, y1, x2, y2);

  for (int y = y1; y <= y2; y++) {
    for (int x = x1; x <= x2; x++) {
      /* The order of bits is MSB first
                  MSB           LSB
         bits      7 6 5 4 3 2 1 0
         pixels    0 1 2 3 4 5 6 7
                  Left         Right
      */
      bool chroma_color =
          (px_map[(hor_res >> 3) * y + (x >> 3)] & 1 << (7 - x % 8));

      /* Write to the buffer as required for the display.
       * It writes only 1-bit for monochrome displays mapped vertically.*/
      uint8_t *buf = oled_buffer + hor_res * (y >> 3) + (x);
      if (chroma_color) {
        (*buf) &= ~(1 << (y % 8));
      } else {
        (*buf) |= (1 << (y % 8));
      }
    }
  }
  // pass the draw buffer to the driver
  esp_lcd_panel_draw_bitmap(panel_handle, x1, y1, x2 + 1, y2 + 1, oled_buffer);
}

void OledDisplay::init() {
  lv_init();

  configure_i2c();
  ssd1306_init();

  display_ = lv_display_create(OLED_WIDTH, OLED_HEIGHT);
  lv_display_set_user_data(display_, panel_handle_);
  lv_display_set_color_format(display_, LV_COLOR_FORMAT_I1);
  lv_display_set_buffers(display_, buffer_, NULL, buffer_size,
                         LV_DISPLAY_RENDER_MODE_PARTIAL);
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
}

void OledDisplay::refresh() {}
void OledDisplay::show_splash_screen() {}
void OledDisplay::show_network_status(bool connected) {}
void OledDisplay::show_idle_screen() {}
void OledDisplay::show_active_screen() {}
void OledDisplay::show_error_screen(const char *error) {}