#ifndef _UTILS_RELAIS_SETUP
#define _UTILS_RELAIS_SETUP

#include <Arduino.h>

#include "../../events/button/event.h"
#include "../../events/button/main.h"
#include "../udp/main.h"
#include "./main.h"

namespace IotNode {
namespace Utils {

namespace Relais {
  extern Class relais0;
  extern Class relais1;

  void setup(UDP::Class *udp, Events::Button::Class *buttons);
}

} // section namespace
} // project namespace

#endif
