#ifndef __AUDIO_H_
#define __AUDIO_H_

#include <esp_event.h>

namespace walle {
class audio {
public:
  void init_audio();

private:
  void play_test();
};
} // namespace walle
#endif