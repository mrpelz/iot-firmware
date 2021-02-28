#ifndef _HELLO_SETUP
#define _HELLO_SETUP

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {

namespace Hello {
  void setup(UDP::Class *udp);
}

} // project namespace

#endif
