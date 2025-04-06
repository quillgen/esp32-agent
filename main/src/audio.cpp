#include "audio.h"
#include <driver/gpio.h>
#include <driver/i2s.h>
#include <esp_log.h>

#include "wav.h"
#include <string.h>

using namespace walle;

#define TAG "🤖 Audio"

#define I2S_BCLK 15
#define I2S_LRC 16
#define I2S_DOUT 7

void audio::init_audio() {
  i2s_config_t i2s_config = {.mode =
                                 (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
                             .sample_rate = 44100,
                             .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
                             .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
                             .communication_format = I2S_COMM_FORMAT_STAND_I2S,
                             .intr_alloc_flags = 0,
                             .dma_buf_count = 8,
                             .dma_buf_len = 64,
                             .use_apll = false,
                             .tx_desc_auto_clear = true};

  i2s_pin_config_t pin_config = {.bck_io_num = I2S_BCLK,
                                 .ws_io_num = I2S_LRC,
                                 .data_out_num = I2S_DOUT,
                                 .data_in_num = I2S_PIN_NO_CHANGE};

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);

  play_test();
}

void audio::play_test() {
  size_t bytes_written;
  for (uint32_t i = 0; i < audio_length; i++) {
    int16_t sample;
    memcpy(&sample, &audio_data[i], sizeof(sample)); // 安全读取方式

    // 发送立体声数据
    int16_t stereo_sample[2] = {sample, sample};
    i2s_write(I2S_NUM_0, stereo_sample, sizeof(stereo_sample), &bytes_written,
              portMAX_DELAY);
  }
}