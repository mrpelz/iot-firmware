#ifndef _BME280_SETUP
#define _BME280_SETUP

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {

namespace Bme280 {
  void setup(UDP::Class *udp);
}

} // project namespace

#endif
