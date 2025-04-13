#ifndef __NETWORK_H_
#define __NETWORK_H_

#include <esp_event.h>

namespace agent {
class network {
public:
  network(EventGroupHandle_t e);

public:
  void init_wifi();

private:
  void event_handler(esp_event_base_t event_base, int32_t event_id,
                     void *event_data);
  static void event_handler_static(void *arg, esp_event_base_t base,
                                   int32_t event_id, void *event_data);
  void sync_time();

private:
  int retry_count = 0;
  EventGroupHandle_t event_group;
};
} // namespace agent
#endif