#ifndef _SERVICES_SYSTEM_INFO_SETUP
#define _SERVICES_SYSTEM_INFO_SETUP

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace SystemInfo {
  void setup(Utils::UDP::Class *udp);
}

} // section namespace
} // project namespace

#endif
