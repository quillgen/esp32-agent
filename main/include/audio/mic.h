#ifndef __MIC_H_
#define __MIC_H_

namespace agent {
class Mic {
public:
  void init();
  void read_sound();

private:
  void configure_i2c();
};
} // namespace agent
#endif