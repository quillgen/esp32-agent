#include "audio/speaker.h"
#include "audio/codec.h"
#include "device.h"
#include <driver/gpio.h>
#include <driver/i2s.h>
#include <esp_check.h>
#include <esp_log.h>
#include <math.h>
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

// Robust WAV header parser
bool parse_wav_header(FILE *f, wav_info_t *info) {
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
  // Only support PCM (audio_format == 1)
  return found_fmt && found_data && audio_format == 1;
}

static void i2s_init(const wav_info_t *info) {
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

void play_wav_file(const char *path) {
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

  i2s_init(&info);

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

#define MAX_HTTP_RECV_BUFFER 512

void speaker::init_speaker() {}

wav_header_t wav_header;
bool header_parsed = false;
size_t total_bytes_read = 0;

esp_err_t speaker::http_event_handler(esp_http_client_event_t *e) {
  switch (e->event_id) {
  case HTTP_EVENT_ON_CONNECTED:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
    break;
  case HTTP_EVENT_HEADER_SENT:
    ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
    break;
  case HTTP_EVENT_ON_HEADER:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", e->header_key,
             e->header_value);
    break;
  case HTTP_EVENT_ON_DATA:
    ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", e->data_len);
    if (!header_parsed) {
      // Accumulate first 44 bytes (WAV header)
      memcpy((uint8_t *)&wav_header + total_bytes_read, e->data, e->data_len);
      total_bytes_read += e->data_len;

      if (total_bytes_read >= sizeof(wav_header)) {
        header_parsed = true;
        ESP_LOGI(TAG, "WAV header parsed. Sample rate: %d",
                 wav_header.sample_rate);
      }
    } else {
      // Stream PCM data to I2S
      size_t bytes_written;
      int16_t *raw_data = (int16_t *)e->data;
      size_t sample_count = e->data_len / sizeof(int16_t);
      for (int i = 0; i < sample_count; i++) {
        raw_data[i] = raw_data[i] * 0.1f; // audio gain
      }
      i2s_write(I2S_NUM_0, e->data, e->data_len, &bytes_written, portMAX_DELAY);
    }
    break;
  case HTTP_EVENT_ON_FINISH:
    ESP_LOGI(TAG, "HTTP transfer completed");
    break;
  case HTTP_EVENT_DISCONNECTED:
    ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
    break;
  case HTTP_EVENT_ERROR:
    ESP_LOGE(TAG, "HTTP error");
    break;
  case HTTP_EVENT_REDIRECT:
    ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
    break;
  default:
    break;
  }

  return ESP_OK;
}

#define WAV_FILE "/spiffs/Vgraata.wav"
#define BUFFER_SIZE 4096

static void play_wav_task(void *arg) {
  ESP_LOGI(TAG, "Opening WAV file");
  FILE *file = fopen(WAV_FILE, "rb");
  if (!file) {
    ESP_LOGE(TAG, "Failed to open file");
    vTaskDelete(NULL);
  }

  // 读取并验证WAV头
  wav_header_t wav_header;
  if (fread(&wav_header, sizeof(wav_header), 1, file) != 1) {
    ESP_LOGE(TAG, "File read error");
    fclose(file);
    vTaskDelete(NULL);
  }

  // 验证WAV格式
  if (memcmp(wav_header.chunk_id, "RIFF", 4) ||
      memcmp(wav_header.format, "WAVE", 4)) {
    ESP_LOGE(TAG, "Invalid WAV file");
    fclose(file);
    vTaskDelete(NULL);
  }

  ESP_LOGI(TAG, "WAV: %dHz, %d bits, %d channels", wav_header.sample_rate,
           wav_header.bits_per_sample, wav_header.num_channels);

  // 跳过扩展块（如果有）
  size_t header_size = sizeof(wav_header);
  while (memcmp(wav_header.data_subchunk_id, "data", 4) != 0) {
    fseek(file, wav_header.data_subchunk_size, SEEK_CUR);
    fread(&wav_header.data_subchunk_id, 4, 1, file);
    fread(&wav_header.data_subchunk_size, 4, 1, file);
    header_size += 8;
  }

  ESP_LOGI(TAG, "Data size: %d bytes", wav_header.data_subchunk_size);

  // 准备音频缓冲区
  uint8_t *buffer = (uint8_t *)malloc(BUFFER_SIZE);
  if (!buffer) {
    ESP_LOGE(TAG, "Malloc failed");
    fclose(file);
    vTaskDelete(NULL);
  }

  // 音频播放循环
  size_t bytes_read;
  size_t bytes_written;
  uint32_t total_bytes_read = 0;

  while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
    // 更新总字节数
    total_bytes_read += bytes_read;

    // 写入I2S
    i2s_write(I2S_NUM_0, buffer, bytes_read, &bytes_written, portMAX_DELAY);

    // 检查是否播放完成
    if (total_bytes_read >= wav_header.data_subchunk_size) {
      break;
    }
  }

  // 清理
  free(buffer);
  fclose(file);
  ESP_LOGI(TAG, "Playback finished");
}
void speaker::test() {
  /**
   * generate test audio:
   * ffmpeg -i 1.mp3 -ar 16000 -ac 2 -acodec pcm_s16le 1.wav
   */
  // xTaskCreate(speaker::http_stream_task, "http_stream", 4096 * 2, nullptr, 5,
  //             nullptr);
  // xTaskCreate(play_wav_task, "play_mp3_task", 4096 * 2, NULL, 5, NULL);
  xTaskCreate(
      [](void *) {
        play_wav_file("/spiffs/Vgramoa.wav");
        vTaskDelete(NULL);
      },
      "wav_player", 4096, NULL, 5, NULL);
}

void speaker::http_stream_task(void *pvParameters) {
  char *buffer = new char[MAX_HTTP_RECV_BUFFER + 1];
  esp_http_client_config_t config = {
      .url = "http://192.168.31.66:8080/1.wav",
      .timeout_ms = 10000,
      .disable_auto_redirect = false,
      .event_handler = speaker::http_event_handler,
      .buffer_size = 1024,
  };

  esp_http_client_handle_t client = esp_http_client_init(&config);
  esp_err_t err;
  if ((err = esp_http_client_open(client, 0)) != ESP_OK) {
    ESP_LOGE(TAG, "failed to open HTTP connection: %s", esp_err_to_name(err));
    // free(buffer);
    return;
  }
  int content_length = esp_http_client_fetch_headers(client);
  int total_read_len = 0, read_len;
  if (total_read_len < content_length &&
      content_length <= MAX_HTTP_RECV_BUFFER) {
    read_len = esp_http_client_read(client, buffer, content_length);
    if (read_len <= 0) {
      ESP_LOGE(TAG, "Error read data");
    }
    buffer[read_len] = 0;
    ESP_LOGD(TAG, "read_len = %d", read_len);
  }

  err = esp_http_client_perform(client);
  if (err == ESP_OK) {
    ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %" PRId64,
             esp_http_client_get_status_code(client),
             esp_http_client_get_content_length(client));
  } else {
    ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
  }
  esp_http_client_close(client);
  esp_http_client_cleanup(client);
  delete[] buffer;
  vTaskDelete(NULL);
}