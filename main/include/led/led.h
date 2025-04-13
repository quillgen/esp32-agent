#ifndef __LED_H_
#define __LED_H_

#include "device.h"

namespace agent {
class Led : public Device {
public:
  virtual ~Led() = default;

public:
  virtual void on_state_changed() = 0;
  void init() override {}
};
} // namespace agent

#endif