#ifndef _BME280_SETUP
#define _BME280_SETUP

#ifdef IOT_NODE_BME280

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {

namespace Bme280 {
  void setup(UDP::Class *udp);
}

} // project namespace

#endif

#endif
