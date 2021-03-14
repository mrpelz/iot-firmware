#ifndef _SERVICES_CCS811_SETUP
#define _SERVICES_CCS811_SETUP

#ifdef IOT_NODE_CCS811

#include <Arduino.h>

#include "../../utils/i2c.h"
#include "../../utils/udp/main.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Ccs811 {
  void setup();
}

} // section namespace
} // project namespace

#endif

#endif
