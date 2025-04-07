#ifndef __DEVICE_H_
#define __DEVICE_H_

namespace walle {
class device {
public:
  virtual ~device() = default;

public:
  virtual void init() = 0;
};
} // namespace walle
#endif