#ifndef __AUDIO_H_
#define __AUDIO_H_

#include <esp_event.h>
#include <esp_http_client.h>

namespace agent {
class speaker {
public:
  void init_speaker();
  void test();
  static void http_stream_task(void *pvParameters);
  static esp_err_t http_event_handler(esp_http_client_event_t *e);
};
} // namespace agent
#endif