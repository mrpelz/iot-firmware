#ifndef _SERVICES_RELAIS_SETUP
#define _SERVICES_RELAIS_SETUP

#ifdef IOT_NODE_RELAIS

#include <Arduino.h>

#include "../../utils/relais/setup.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Relais {
  void setup();
}

} // section namespace
} // project namespace

#endif

#endif
