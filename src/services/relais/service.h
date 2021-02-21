#ifndef _RELAIS_SERVICE
#define _RELAIS_SERVICE

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "../service-ids.h"
#include "./main.h"

namespace IotNode {

namespace Relais {
  UDP::Service makeService(Class *relais, uint8_t index);
}

} // project namespace

#endif
