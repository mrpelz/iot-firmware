#ifndef _SERVICES_LED_MAIN
#define _SERVICES_LED_MAIN

#ifdef IOT_NODE_LED

#include <FadeLed.h>

#include "../../utils/udp/main.h"
#include "../service-ids.h"

namespace IotNode {
namespace Services {

namespace Led {
  Utils::UDP::Service makeService(FadeLed *led, uint8_t index);
}

} // section namespace
} // project namespace

#endif

#endif
