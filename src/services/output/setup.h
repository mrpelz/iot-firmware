#ifndef _SERVICES_OUTPUT_SETUP
#define _SERVICES_OUTPUT_SETUP

#ifdef IOT_NODE_OUTPUT

#include <Arduino.h>

#include "../../utils/output/setup.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Output {
  void setup();
}

} // section namespace
} // project namespace

#endif

#endif
