#ifndef _KEEPALIVE_HANDLER
#define _KEEPALIVE_HANDLER

#include <Arduino.h>
#include "../../utils/udp/main.h"
#include "../../utils/log.h"

namespace IotNode {

struct RestartOnTimeoutState {
  bool ticked = false;
  bool running = false;
  uint32_t timeout = 0;
  uint32_t lastTick = 0;
};

class RestartOnTimeout {
  private:
    RestartOnTimeoutState state;

  public:
    RestartOnTimeout(uint32_t timeout);
    void start();
    void stop();
    void tick();
    void update();
};

void keepAliveHandler(
    UDP::Payload *request,
    UDP::RespondCallback respond
);

void timeoutUpdate();

} // project namespace

#endif
