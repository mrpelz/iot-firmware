#ifndef _SYSTEM_INFO_SETUP
#define _SYSTEM_INFO_SETUP

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {

namespace SystemInfo {
  void setup(UDP::Class *udp);
}

} // project namespace

#endif
