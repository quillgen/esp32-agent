#ifndef __DEVICE_H_
#define __DEVICE_H_

namespace agent {
class Device {
public:
  virtual ~Device() = default;

public:
  virtual void init() = 0;
};
} // namespace agent
#endif