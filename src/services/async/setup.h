#ifndef _ASYNC_SETUP
#define _ASYNC_SETUP

#ifdef ARDUINO_ARCH_ESP32

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {

namespace Async {
  void setup(UDP::Class *udp);
}

} // project namespace

#endif

#endif
