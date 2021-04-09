#ifndef _UTILS_KEEPALIVE_MAIN
#define _UTILS_KEEPALIVE_MAIN

#include <Arduino.h>
#include <functional>

#include "../../utils/log.h"

namespace IotNode {
namespace Utils {

namespace Keepalive {
  typedef std::function<void ()> KeepaliveCallback;

  struct State {
    bool running = false;
    bool ticked = false;
    KeepaliveCallback callback;
    unsigned long lastTick = 0;
    unsigned long timeout = 0;
  };

  class Class {
    private:
      State state;

    public:
      Class(unsigned long timeout, KeepaliveCallback callback);
      void start();
      void stop();
      void tick();
      void update();
  };
}

} // section namespace
} // project namespace

#endif
