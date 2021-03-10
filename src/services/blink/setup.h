#ifndef _BLINK_SETUP
#define _BLINK_SETUP

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {

namespace Blink {
  void setup(UDP::Class *udp);
}

} // project namespace

#endif
