#ifndef __LED_H_
#define __LED_H_

#include "device.h"
#include "event.h"

namespace agent {
class Led : public Device {
public:
  virtual ~Led() = default;

public:
  virtual void on_state_changed(AppState s) = 0;
  void init() override {}
};
} // namespace agent

#endif