#ifndef _SERVICES_KEEPALIVE_MAIN
#define _SERVICES_KEEPALIVE_MAIN

#include <Arduino.h>

#include "../../utils/keepalive/setup.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"
#include "../../utils/udp/setup.h"
#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Keepalive {
  Utils::UDP::RequestHandler makeHandler();
}

} // section namespace
} // project namespace

#endif
