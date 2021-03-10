#ifndef _SERVICES_ASYNC_SETUP
#define _SERVICES_ASYNC_SETUP

#ifdef ARDUINO_ARCH_ESP32

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Async {
  void setup(Utils::UDP::Class *udp);
}

} // section namespace
} // project namespace

#endif

#endif
