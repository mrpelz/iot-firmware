#ifndef _TSL2561_SETUP
#define _TSL2561_SETUP

#ifdef IOT_NODE_TSL2561

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {

namespace Tsl2561 {
  void setup(UDP::Class *udp);
}

} // project namespace

#endif

#endif
