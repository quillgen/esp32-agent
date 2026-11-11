#include "audio/speaker.h"
#include "device.h"

#include <driver/i2s_std.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace agent;

#define TAG "speaker"

#define I2S_NUM (I2S_NUM_0)

typedef struct {
  uint32_t sample_rate;
  uint16_t bits_per_sample;
  uint16_t channels;
  uint32_t data_offset;
  uint32_t data_size;
} wav_info_t;

static i2s_chan_handle_t s_tx_chan = nullptr;

// Helper to read little-endian 4-byte integer
static uint32_t read_le_uint32(FILE *f) {
  uint8_t b[4];
  fread(b, 1, 4, f);
  return b[0] | (b[1] << 8) | (b[2] << 16) | (b[3] << 24);
}

// Helper to read little-endian 2-byte integer
static uint16_t read_le_uint16(FILE *f) {
  uint8_t b[2];
  fread(b, 1, 2, f);
  return b[0] | (b[1] << 8);
}

// Robust WAV header parser. Only PCM (audio_format == 1) is supported.
static bool parse_wav_header(FILE *f, wav_info_t *info) {
  uint8_t riff_header[12];
  if (fread(riff_header, 1, 12, f) != 12)
    return false;
  if (memcmp(riff_header, "RIFF", 4) || memcmp(riff_header + 8, "WAVE", 4))
    return false;

  // Look for "fmt " chunk
  char chunk_id[5] = {0};
  uint32_t chunk_size;
  bool found_fmt = false, found_data = false;
  uint16_t audio_format = 0;
  while (!found_data) {
    if (fread(chunk_id, 1, 4, f) != 4)
      return false;
    chunk_size = read_le_uint32(f);

    if (!memcmp(chunk_id, "fmt ", 4)) {
      found_fmt = true;
      audio_format = read_le_uint16(f); // PCM=1
      info->channels = read_le_uint16(f);
      info->sample_rate = read_le_uint32(f);
      fseek(f, 6, SEEK_CUR); // Skip byte rate and block align
      info->bits_per_sample = read_le_uint16(f);
      // Skip any extra fmt bytes
      if (chunk_size > 16)
        fseek(f, chunk_size - 16, SEEK_CUR);
    } else if (!memcmp(chunk_id, "data", 4)) {
      info->data_offset = ftell(f);
      info->data_size = chunk_size;
      found_data = true;
      break;
    } else {
      // Unknown chunk, skip it
      fseek(f, chunk_size, SEEK_CUR);
    }
  }
  return found_fmt && found_data && audio_format == 1;
}

// (Re)create the I2S TX channel for the given format. Destroys any existing
// channel first, since slot width/channels/sample rate are fixed per channel.
static void i2s_configure(const wav_info_t *info) {
  if (s_tx_chan != nullptr) {
    i2s_channel_disable(s_tx_chan);
    i2s_del_channel(s_tx_chan);
    s_tx_chan = nullptr;
  }

  i2s_chan_config_t chan_cfg =
      I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM, I2S_ROLE_MASTER);
  ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &s_tx_chan, nullptr));

  i2s_std_config_t std_cfg = {
      .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(info->sample_rate),
      .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(
          (info->bits_per_sample == 16) ? I2S_DATA_BIT_WIDTH_16BIT
                                        : I2S_DATA_BIT_WIDTH_8BIT,
          (info->channels == 2) ? I2S_SLOT_MODE_STEREO : I2S_SLOT_MODE_MONO),
      .gpio_cfg =
          {
              .mclk = I2S_GPIO_UNUSED,
              .bclk = (gpio_num_t)SPEAKER_I2S_BCLK,
              .ws = (gpio_num_t)SPEAKER_I2S_LRC,
              .dout = (gpio_num_t)SPEAKER_I2S_DOUT,
              .din = I2S_GPIO_UNUSED,
              .invert_flags =
                  {
                      .mclk_inv = false,
                      .bclk_inv = false,
                      .ws_inv = false,
                  },
          },
  };
  ESP_ERROR_CHECK(i2s_channel_init_std_mode(s_tx_chan, &std_cfg));
  ESP_ERROR_CHECK(i2s_channel_enable(s_tx_chan));
}

void Speaker::init_speaker() {
  // Install the I2S TX channel with sane defaults. play_wav_file() recreates
  // the channel when a file uses a different format.
  wav_info_t defaults = {};
  defaults.sample_rate = 16000;
  defaults.bits_per_sample = 16;
  defaults.channels = 2;
  i2s_configure(&defaults);
}

static void play_wav_file(const char *path) {
  FILE *f = fopen(path, "rb");
  if (!f) {
    ESP_LOGE(TAG, "Failed to open: %s", path);
    return;
  }

  wav_info_t info;
  if (!parse_wav_header(f, &info)) {
    ESP_LOGE(TAG, "Bad WAV header or unsupported format");
    fclose(f);
    return;
  }

  ESP_LOGI(TAG, "WAV: %d Hz, %d bits, %d channels, offset=%d size=%d",
           info.sample_rate, info.bits_per_sample, info.channels,
           info.data_offset, info.data_size);

  i2s_configure(&info);

  // Seek to audio data
  fseek(f, info.data_offset, SEEK_SET);

  const size_t buf_size = 1024;
  uint8_t *buf = (uint8_t *)malloc(buf_size);
  if (!buf) {
    ESP_LOGE(TAG, "Buffer alloc failed");
    fclose(f);
    return;
  }

  size_t to_read = info.data_size;
  size_t bytes_written;
  while (to_read > 0) {
    size_t chunk = (to_read > buf_size) ? buf_size : to_read;
    size_t read = fread(buf, 1, chunk, f);
    if (read <= 0)
      break;

    esp_err_t ret =
        i2s_channel_write(s_tx_chan, buf, read, &bytes_written, portMAX_DELAY);

    ESP_LOGI(TAG, "Read %zu bytes, written %zu bytes, ret=%d", read,
             bytes_written, ret);
    to_read -= read;
  }

  // Flush the tail with silence so no noise is played afterwards
  vTaskDelay(100 / portTICK_PERIOD_MS);
  int16_t silence[256] = {0};
  esp_err_t ret = i2s_channel_write(s_tx_chan, silence, sizeof(silence),
                                    &bytes_written, portMAX_DELAY);
  ESP_LOGI(TAG, "written %zu silent bytes, ret=%d", bytes_written, ret);

  free(buf);
  fclose(f);
  ESP_LOGI(TAG, "Playback finished");
}

void Speaker::test() {
  /**
   * generate test audio:
   * ffmpeg -i 1.mp3 -ar 16000 -ac 2 -acodec pcm_s16le 1.wav
   */
  xTaskCreate(
      [](void *) {
        play_wav_file("/spiffs/Vgramoa.wav");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        play_wav_file("/spiffs/Vgramoc.mono.wav");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        play_wav_file("/spiffs/Vgramoc.wav");
        vTaskDelete(NULL);
      },
      "wav_player", 4096, NULL, 5, NULL);
}
