#ifndef _RELAIS_SETUP
#define _RELAIS_SETUP

#include <Arduino.h>

#include "../../events/button/event.h"
#include "../../events/button/main.h"
#include "../../utils/udp/main.h"
#include "./main.h"
#include "./service.h"

namespace IotNode {

namespace Relais {
  void setup(UDP::Class *udp, Button::Class *buttons);
}

} // project namespace

#endif
