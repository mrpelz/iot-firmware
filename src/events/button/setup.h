#ifndef _EVENTS_BUTTON_SETUP
#define _EVENTS_BUTTON_SETUP

#include <Arduino.h>

#include "../../utils/button/setup.h"
#include "../../utils/log.h"
#include "../../utils/relais/setup.h"
#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {
namespace Events {

namespace Button {
  void setup(Utils::UDP::Class *udp);
}

} // section namespace
} // project namespace

#endif
