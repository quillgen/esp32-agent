#ifndef __DEVICE_H_
#define __DEVICE_H_

#define OLED_I2C_SDA_PIN 4
#define OLED_I2C_SCL_PIN 5

#define SPEAKER_I2S_DOUT 6
#define SPEAKER_I2S_BCLK 7
#define SPEAKER_I2S_LRC 15

#define MIC_I2S_WS 16   // WS (LRCLK)
#define MIC_I2S_BCLK 17 // SCK(BCLK)
#define MIC_I2S_DATA 18 // SD

namespace agent {
class Device {
public:
  virtual ~Device() = default;

public:
  virtual void init() = 0;
};
} // namespace agent
#endif