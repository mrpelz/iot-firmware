#ifndef _KEEPALIVE_SETUP
#define _KEEPALIVE_SETUP

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {

namespace Keepalive {
  void update();

  #ifdef ARDUINO_ARCH_ESP32
    void task(void *parameter);
  #endif

  void setup(UDP::Class *udp);
}

} // project namespace

#endif
