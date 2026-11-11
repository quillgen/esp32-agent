#include "audio/mic.h"
#include "device.h"

#include <driver/i2s_std.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

using namespace agent;

#define TAG "mic"

static i2s_chan_handle_t s_rx_chan = nullptr;

void Mic::init() { configure_i2c(); }

void Mic::configure_i2c() {
  i2s_chan_config_t chan_cfg =
      I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_1, I2S_ROLE_MASTER);
  ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, nullptr, &s_rx_chan));

  // INMP441 outputs 24-bit samples inside 32-bit slots, so receive 32-bit
  // words and shift right by 8 in the consumer.
  i2s_std_config_t std_cfg = {
      .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(44100),
      .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT,
                                                      I2S_SLOT_MODE_MONO),
      .gpio_cfg =
          {
              .mclk = I2S_GPIO_UNUSED,
              .bclk = (gpio_num_t)MIC_I2S_BCLK,
              .ws = (gpio_num_t)MIC_I2S_WS,
              .dout = I2S_GPIO_UNUSED,
              .din = (gpio_num_t)MIC_I2S_DATA,
              .invert_flags =
                  {
                      .mclk_inv = false,
                      .bclk_inv = false,
                      .ws_inv = false,
                  },
          },
  };
  ESP_ERROR_CHECK(i2s_channel_init_std_mode(s_rx_chan, &std_cfg));
  ESP_ERROR_CHECK(i2s_channel_enable(s_rx_chan));
}

void Mic::read_sound() {
  int32_t buffer[128];
  size_t bytes_read;

  i2s_channel_read(s_rx_chan, buffer, sizeof(buffer), &bytes_read,
                   portMAX_DELAY);

  int32_t sample = buffer[0] >> 8; // INMP441: valid data in the upper 24 bits
  ESP_LOGD(TAG, "mic sample: %ld", (long)sample);
  ESP_LOGI(TAG, "read data from mic!");
}
