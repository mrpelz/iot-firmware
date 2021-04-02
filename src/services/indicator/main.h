#ifndef _SERVICES_INDICATOR_MAIN
#define _SERVICES_INDICATOR_MAIN

#ifdef IOT_NODE_INDICATORS

#include <Arduino.h>

#include "../../utils/indicator/setup.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"

#define INDICATOR_CMD_OFF 0
#define INDICATOR_CMD_ON 1
#define INDICATOR_CMD_BLINK 2

namespace IotNode {
namespace Services {

namespace Indicator {
  Utils::UDP::Service makeService(Utils::Indicator::Class *relais, uint8_t index);
}

} // section namespace
} // project namespace

#endif

#endif
