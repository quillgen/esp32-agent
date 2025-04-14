#include "audio/mic.h"
#include "device.h"

#include <driver/i2s.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

using namespace agent;

#define TAG "mic"

void Mic::init() { configure_i2c(); }
void Mic::configure_i2c() {
  i2s_config_t i2s_config = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX), // 主模式，输入
      .sample_rate = 44100,                                // 采样率（Hz）
      .bits_per_sample =
          I2S_BITS_PER_SAMPLE_32BIT, // INMP441 输出 24 位数据，需用 32 位接收
      .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,       // 单声道（左）
      .communication_format = I2S_COMM_FORMAT_STAND_I2S, // 标准 I2S 格式
      .intr_alloc_flags = 0,
      .dma_buf_count = 8, // DMA 缓冲区数量
      .dma_buf_len = 64,  // 缓冲区长度
      .use_apll = false,  // 禁用 APLL
      .tx_desc_auto_clear = false,
      .mclk_multiple = I2S_MCLK_MULTIPLE_256 // 主时钟分频
  };

  // 引脚配置
  i2s_pin_config_t pin_config = {
      .bck_io_num = MIC_I2S_BCLK,
      .ws_io_num = MIC_I2S_WS,
      .data_out_num = I2S_PIN_NO_CHANGE,
      .data_in_num = MIC_I2S_DATA,
  };

  // 初始化 I2S
  ESP_ERROR_CHECK(i2s_driver_install(I2S_NUM_1, &i2s_config, 0, NULL));
  ESP_ERROR_CHECK(i2s_set_pin(I2S_NUM_1, &pin_config));
}

void Mic::read_sound() {
  int32_t buffer[128];
  size_t bytes_read;

  i2s_read(I2S_NUM_1, buffer, sizeof(buffer), &bytes_read, portMAX_DELAY);

  int32_t sample = buffer[0] >> 8; // INMP441 的有效数据在高 24 位
  ESP_LOGI(TAG, "read data from mic!");
}