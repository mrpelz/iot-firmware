#ifndef _SERVICES_INDICATOR_MAIN
#define _SERVICES_INDICATOR_MAIN

#include <Arduino.h>

#include "../../utils/indicator/setup.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"

namespace IotNode {
namespace Services {

namespace Indicator {
  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond);
}

} // section namespace
} // project namespace

#endif
