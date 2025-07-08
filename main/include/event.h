#ifndef __EVENT_H_
#define __EVENT_H_

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>

namespace agent {
enum AppState {
  UNKNOWN,
  BOOTING,
  NETWORK_CONNECTING,
  NETWORK_CONNECTED,
  IDLE,
  ACTIVE,
  APP_ERROR,
};

enum NetworkState { DISCONNECTED, CONNECTING, CONNECTED, NETWORK_ERROR };

class Events {
public:
  static constexpr EventBits_t APP_STATE_CHANGED = BIT0;
  static constexpr EventBits_t NETWORK_STATE_CHANGED = BIT1;
};

} // namespace agent

#endif