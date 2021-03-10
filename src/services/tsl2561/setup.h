#ifndef _SERVICES_TSL2561_SETUP
#define _SERVICES_TSL2561_SETUP

#ifdef IOT_NODE_TSL2561

#include <Arduino.h>
#include <Wire.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Tsl2561 {
  void setup(Utils::UDP::Class *udp, TwoWire *i2c);
}

} // section namespace
} // project namespace

#endif

#endif
