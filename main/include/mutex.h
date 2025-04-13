#ifndef __MUTEX_H_
#define __MUTEX_H_

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

namespace agent {
class Mutex {
public:
  Mutex() { handle_ = xSemaphoreCreateMutex(); }
  ~Mutex() { vSemaphoreDelete(handle_); }

  bool lock(TickType_t timeout = portMAX_DELAY) {
    return xSemaphoreTake(handle_, timeout) == pdTRUE;
  }

  void unlock() { xSemaphoreGive(handle_); }

private:
  SemaphoreHandle_t handle_;
};

} // namespace agent
#endif