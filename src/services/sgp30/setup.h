#ifndef _SERVICES_SGP30_SETUP
#define _SERVICES_SGP30_SETUP

#ifdef IOT_NODE_SGP30

#include <Arduino.h>
#include <Wire.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Sgp30 {
  void setup(Utils::UDP::Class *udp, TwoWire *i2c);
}

} // section namespace
} // project namespace

#endif

#endif
