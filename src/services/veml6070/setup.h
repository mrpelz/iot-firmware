#pragma once

#ifdef IOT_NODE_VEML6070

#include <Arduino.h>

#include "../../utils/i2c.h"
#include "../../utils/udp/main.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Veml6070 {
  void setup();
}

} // section namespace
} // project namespace

#endif
