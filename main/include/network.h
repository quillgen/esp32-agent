#ifndef __NETWORK_H_
#define __NETWORK_H_

#include "event.h"
#include <esp_event.h>

namespace agent {
class network {
public:
  network(EventGroupHandle_t e);
  ~network();

public:
  void init();

private:
  void event_handler(esp_event_base_t event_base, int32_t event_id,
                     void *event_data);
  static void event_handler_static(void *arg, esp_event_base_t base,
                                   int32_t event_id, void *event_data);
  NetworkState get_state() const { return state_; }

  void set_state(NetworkState newState);

private:
  NetworkState state_;
  int retry_count_ = 0;
  EventGroupHandle_t event_group_;
  mutable SemaphoreHandle_t mutex_;
};
} // namespace agent
#endif