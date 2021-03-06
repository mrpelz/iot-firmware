#ifndef _CCS811_SETUP
#define _CCS811_SETUP

#ifdef IOT_NODE_CCS811

#include <Arduino.h>
#include <Wire.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {

namespace Ccs811 {
  void setup(UDP::Class *udp, TwoWire *i2c);
}

} // project namespace

#endif

#endif
