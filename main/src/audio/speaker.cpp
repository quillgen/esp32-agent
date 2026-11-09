#include "audio/speaker.h"
#include "device.h"

#include <driver/i2s.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace agent;

#define TAG "speaker"

#define I2S_NUM (I2S_NUM_0)
#define I2S_DMA_BUF_LEN (512)

typedef struct {
  uint32_t sample_rate;
  uint16_t bits_per_sample;
  uint16_t channels;
  uint32_t data_offset;
  uint32_t data_size;
} wav_info_t;

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

static void i2s_configure(const wav_info_t *info) {
  esp_err_t err = i2s_driver_uninstall(I2S_NUM);
  if (err != ESP_OK && err != ESP_ERR_NOT_FOUND) {
    ESP_LOGE(TAG, "Failed to uninstall I2S: %s", esp_err_to_name(err));
  }

  i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
      .sample_rate = info->sample_rate,
      .bits_per_sample = (info->bits_per_sample == 16)
                             ? I2S_BITS_PER_SAMPLE_16BIT
                             : I2S_BITS_PER_SAMPLE_8BIT,
      .channel_format = (info->channels == 2) ? I2S_CHANNEL_FMT_RIGHT_LEFT
                                              : I2S_CHANNEL_FMT_ONLY_LEFT,
      .communication_format = I2S_COMM_FORMAT_I2S_MSB,
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
      .dma_buf_count = 8,
      .dma_buf_len = I2S_DMA_BUF_LEN,
  };

  i2s_pin_config_t pin_config = {.bck_io_num = SPEAKER_I2S_BCLK,
                                 .ws_io_num = SPEAKER_I2S_LRC,
                                 .data_out_num = SPEAKER_I2S_DOUT,
                                 .data_in_num = I2S_PIN_NO_CHANGE};

  i2s_driver_install(I2S_NUM, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM, &pin_config);
  i2s_zero_dma_buffer(I2S_NUM);
}

void Speaker::init_speaker() {
  // Install the I2S TX driver with sane defaults. play_wav_file() reconfigures
  // the driver when a file uses a different format.
  const wav_info_t defaults = {
      .sample_rate = 16000,
      .bits_per_sample = 16,
      .channels = 2,
  };
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

  const size_t buf_size =
      I2S_DMA_BUF_LEN * ((info.bits_per_sample == 16) ? 2 : 1) * info.channels;
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
        i2s_write(I2S_NUM, buf, read, &bytes_written, portMAX_DELAY);

    ESP_LOGI(TAG, "Read %zu bytes, written %zu bytes, ret=%d", read,
             bytes_written, ret);
    to_read -= read;
  }
  vTaskDelay(100 / portTICK_PERIOD_MS);
  int16_t silence[256] = {0};
  esp_err_t ret = i2s_write(I2S_NUM, silence, sizeof(silence), &bytes_written,
                            portMAX_DELAY);
  ESP_LOGI(TAG, "written %zu silent bytes, ret=%d", bytes_written, ret);

  free(buf);
  fclose(f);

  // make sure to clear the I2S buffer so that no noise is played after
  i2s_zero_dma_buffer(I2S_NUM);
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
