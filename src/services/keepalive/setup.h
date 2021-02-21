#ifndef _KEEPALIVE_SETUP
#define _KEEPALIVE_SETUP

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {

namespace Keepalive {
  void task(void * parameter);

  void setup(UDP::Class *udp);
}

} // project namespace

#endif
