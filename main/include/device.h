#ifndef __DEVICE_H_
#define __DEVICE_H_

#define I2C_SDA_PIN 4
#define I2C_SCL_PIN 5

#define I2S_DOUT 6
#define I2S_BCLK 7
#define I2S_LRC 15

namespace agent {
class Device {
public:
  virtual ~Device() = default;

public:
  virtual void init() = 0;
};
} // namespace agent
#endif