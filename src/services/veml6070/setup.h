#ifndef _SERVICES_VEML6070_SETUP
#define _SERVICES_VEML6070_SETUP

#ifdef IOT_NODE_VEML6070

#include <Arduino.h>
#include <Wire.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Veml6070 {
  void setup(Utils::UDP::Class *udp, TwoWire *i2c);
}

} // section namespace
} // project namespace

#endif

#endif
