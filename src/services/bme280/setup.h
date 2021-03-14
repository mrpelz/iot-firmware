#ifndef _SERVICES_BME280_SETUP
#define _SERVICES_BME280_SETUP

#ifdef IOT_NODE_BME280

#include <Arduino.h>

#include "../../utils/i2c.h"
#include "../../utils/udp/main.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Bme280 {
  void setup();
}

} // section namespace
} // project namespace

#endif

#endif
