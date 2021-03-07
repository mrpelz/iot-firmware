#ifndef _VEML6070_SETUP
#define _VEML6070_SETUP

#ifdef IOT_NODE_VEML6070

#include <Arduino.h>
#include <Wire.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {

namespace Veml6070 {
  void setup(UDP::Class *udp, TwoWire *i2c);
}

} // project namespace

#endif

#endif
