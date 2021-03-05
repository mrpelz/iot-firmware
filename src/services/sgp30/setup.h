#ifndef _SGP30_SETUP
#define _SGP30_SETUP

#ifdef IOT_NODE_SGP30

#include <Arduino.h>
#include <Wire.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {

namespace Sgp30 {
  void setup(UDP::Class *udp, TwoWire *i2c);
}

} // project namespace

#endif

#endif
