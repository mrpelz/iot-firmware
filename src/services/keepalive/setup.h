#ifndef _SERVICES_KEEPALIVE_SETUP
#define _SERVICES_KEEPALIVE_SETUP

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Keepalive {
  void update();

  #ifdef IOT_NODE_ESP32
    void task(void *parameter);
  #endif

  void setup();
}

} // section namespace
} // project namespace

#endif
