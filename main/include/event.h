#ifndef __EVENT_H_
#define __EVENT_H_

#define BIT_APP_STATE_CHANGED BIT0
#define BIT_WIFI_CONNECTED BIT1
#define BIT_WIFI_FAILED BIT2

namespace agent {
enum AppState {
  kUnknown,
  kNetworkConnecting,
  kNstp,
  kIdle,
  kNetworkNotConnected,
  kError,
};
}

#endif