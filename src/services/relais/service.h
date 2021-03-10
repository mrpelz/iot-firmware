#ifndef _SERVICES_RELAIS_SERVICE
#define _SERVICES_RELAIS_SERVICE

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "../service-ids.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Relais {
  Utils::UDP::Service makeService(Class *relais, uint8_t index);
}

} // section namespace
} // project namespace

#endif
