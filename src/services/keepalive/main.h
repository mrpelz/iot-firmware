#ifndef _KEEPALIVE_MAIN
#define _KEEPALIVE_MAIN

#include <Arduino.h>

#include "../../utils/log.h"
#include "../../utils/udp/main.h"

namespace IotNode {

namespace Keepalive {
  struct State {
    bool ticked = false;
    bool running = false;
    uint32_t timeout = 0;
    uint32_t lastTick = 0;
  };

  class Class {
    private:
      State state;

    public:
      Class(uint32_t timeout);
      void start();
      void stop();
      void tick();
      void update();
  };

  UDP::RequestHandler makeHandler(Class *restartOnTimeout);
}

} // project namespace

#endif
