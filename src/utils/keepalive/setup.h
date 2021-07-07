#pragma once

#include <Arduino.h>

#include "../udp/setup.h"
#include "./main.h"

namespace IotNode {
namespace Utils {

namespace Keepalive {
  extern Class keepalive;
  extern Class eventPeer;

  void update();

  #ifdef IOT_NODE_ESP32
    void task(void *parameter);
  #endif

  void setup();
}

} // section namespace
} // project namespace
