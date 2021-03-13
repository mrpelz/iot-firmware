#ifndef _SERVICES_RELAIS_SETUP
#define _SERVICES_RELAIS_SETUP

#include <Arduino.h>

#include "../../utils/relais/setup.h"
#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Relais {
  void setup(Utils::UDP::Class *udp);
}

} // section namespace
} // project namespace

#endif
