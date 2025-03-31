#ifndef __LED_H_
#define __LED_H_

namespace walle {
class led {
public:
  virtual ~led() = default;

public:
  virtual void on_state_changed() = 0;
};

class no_led : public led {
public:
  virtual void on_state_changed() override {}
};
} // namespace walle

#endif