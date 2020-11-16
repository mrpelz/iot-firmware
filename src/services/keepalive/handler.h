#ifndef _KEEPALIVE_HANDLER
#define _KEEPALIVE_HANDLER

#include <Arduino.h>
#include "../../udp-messaging.h"
#include "../../logging.h"

struct RestartOnTimeoutState {
  bool ticked = false;
  bool running = false;
  unsigned long timeout = 0;
  unsigned long lastTick = 0;
};

class RestartOnTimeout {
  private:
    RestartOnTimeoutState state;

  public:
    RestartOnTimeout(unsigned long timeout);
    void start();
    void stop();
    void tick();
    void update();
};

void keepAliveHandler(
    std::vector<uint8_t> *request,
    RespondCallback respond
);

void TimeoutUpdate();

#endif
