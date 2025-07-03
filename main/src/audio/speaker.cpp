#include "audio/speaker.h"
#include "audio/codec.h"
#include "device.h"
#include <driver/gpio.h>
#include <driver/i2s.h>
#include <math.h>

#include <esp_log.h>

#include <string.h>

using namespace agent;

#define TAG "🤖 Audio"

#define MAX_HTTP_RECV_BUFFER 512

void speaker::init_speaker() {
  i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
      .sample_rate = 16000,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
      .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = I2S_COMM_FORMAT_I2S,
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
      .dma_buf_count = 8,
      .dma_buf_len = 512,
      .use_apll = false,
      .tx_desc_auto_clear = true,
  };

  i2s_pin_config_t pin_config = {.bck_io_num = I2S_BCLK,
                                 .ws_io_num = I2S_LRC,
                                 .data_out_num = I2S_DOUT,
                                 .data_in_num = I2S_PIN_NO_CHANGE};

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
}

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
        raw_data[i] = raw_data[i]; // audio gain
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

void speaker::test() {
  /**
   * generate test audio:
   * ffmpeg -i 1.mp3 -ar 16000 -ac 2 -acodec pcm_s16le 1.wav
   */
  xTaskCreate(speaker::http_stream_task, "http_stream", 4096 * 2, nullptr, 5,
              nullptr);
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