#ifndef _SERVICES_CCS811_SETUP
#define _SERVICES_CCS811_SETUP

#ifdef IOT_NODE_CCS811

#include <Arduino.h>
#include <Wire.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Ccs811 {
  void setup(Utils::UDP::Class *udp, TwoWire *i2c);
}

} // section namespace
} // project namespace

#endif

#endif
