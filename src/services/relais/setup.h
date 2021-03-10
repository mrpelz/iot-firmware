#ifndef _SERVICES_RELAIS_SETUP
#define _SERVICES_RELAIS_SETUP

#include <Arduino.h>

#include "../../events/button/event.h"
#include "../../events/button/main.h"
#include "../../utils/udp/main.h"
#include "./main.h"
#include "./service.h"

namespace IotNode {
namespace Services {

namespace Relais {
  void setup(Utils::UDP::Class *udp, Events::Button::Class *buttons);
}

} // section namespace
} // project namespace

#endif
