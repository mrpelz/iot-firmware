#ifndef _SERVICES_INDICATOR_SETUP
#define _SERVICES_INDICATOR_SETUP

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Indicator {
  void setup(Utils::UDP::Class *udp);
}

} // section namespace
} // project namespace

#endif
