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
    bool forceRestart = false;
    unsigned long timeout = 0;
    unsigned long lastTick = 0;
  };

  class Class {
    private:
      State state;

    public:
      Class(unsigned long timeout);
      void start();
      void stop();
      void tick(bool forceRestart);
      void update();
  };

  UDP::RequestHandler makeHandler(Class *restartOnTimeout);
}

} // project namespace

#endif
