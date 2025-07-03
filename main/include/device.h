#ifndef __DEVICE_H_
#define __DEVICE_H_

#define OLED_I2C_SDA_PIN 5
#define OLED_I2C_SCL_PIN 4

#define SPEAKER_I2S_DOUT 7
#define SPEAKER_I2S_BCLK 15
#define SPEAKER_I2S_LRC 16

#define MIC_I2S_WS 3    // WS (LRCLK)
#define MIC_I2S_BCLK 6  // SCK(BCLK)
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