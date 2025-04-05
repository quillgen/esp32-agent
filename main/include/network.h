#ifndef __NETWORK_H_
#define __NETWORK_H_

#include <esp_event.h>
#include <esp_smartconfig.h>

namespace walle {
class network {
public:
  network(EventGroupHandle_t e);

public:
  void init_wifi();
  bool block_for_connection();

private:
  static void event_handler_static(void *arg, esp_event_base_t base,
                                   int32_t event_id, void *event_data);
  void event_handler(esp_event_base_t event_base, int32_t event_id,
                     void *event_data);
  static void smart_config_task_static(void *parm);
  void smart_config_task();
  void handle_wifi_event(int32_t event_id, void *event_data);
  void handle_ip_event(int32_t event_id, void *event_data);
  void handle_sc_event(int32_t event_id, void *event_data);
  void connect_form_sc(smartconfig_event_got_ssid_pswd_t *e);

private:
  int retry_count = 0;
  EventGroupHandle_t event_group;
};
} // namespace walle
#endif