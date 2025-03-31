#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include "led/led.h"

namespace walle {

enum app_state {
  unknown,
  starting,
  configuring_wifi,
  idle,
  connecting,
  listening,
  speaking,
  upgrading,
  activiting,
  error,
};

class application {
public:
  static application &instance() {
    static application INSTANCE;
    return INSTANCE;
  }

public:
  application(const application &) = delete;
  application &operator=(const application &) = delete;

public:
  inline app_state get_state() { return this->state; }

  void start();
  void reboot();

private:
  application();
  ~application();

private:
  void main_loop();
  void set_state(app_state state);

private:
  volatile app_state state = unknown;
  led *_led = nullptr;
};
} // namespace walle

#endif