#ifndef _SERVICES_BME280_SETUP
#define _SERVICES_BME280_SETUP

#ifdef IOT_NODE_BME280

#include <Arduino.h>
#include <Wire.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Bme280 {
  void setup(Utils::UDP::Class *udp, TwoWire *i2c);
}

} // section namespace
} // project namespace

#endif

#endif
