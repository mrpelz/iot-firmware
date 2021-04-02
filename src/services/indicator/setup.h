#ifndef _SERVICES_INDICATOR_SETUP
#define _SERVICES_INDICATOR_SETUP

#ifdef IOT_NODE_INDICATORS

#include <Arduino.h>

#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Indicator {
  void setup();
}

} // section namespace
} // project namespace

#endif

#endif
