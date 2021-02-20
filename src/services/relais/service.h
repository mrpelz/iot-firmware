#ifndef _RELAIS_SERVICE
#define _RELAIS_SERVICE

#include <Arduino.h>
#include "../../utils/udp/main.h"
#include "../service-ids.h"
#include "./handler.h"

namespace IotNode {

UDP::Service makeRelaisService(Relais *relais, uint8_t index);

} // project namespace

#endif
