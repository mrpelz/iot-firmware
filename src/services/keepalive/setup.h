#ifndef _SERVICES_KEEPALIVE_SETUP
#define _SERVICES_KEEPALIVE_SETUP

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Keepalive {
  void update();

  #ifdef ARDUINO_ARCH_ESP32
    void task(void *parameter);
  #endif

  void setup(Utils::UDP::Class *udp);
}

} // section namespace
} // project namespace

#endif
