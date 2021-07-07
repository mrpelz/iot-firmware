#pragma once

#ifdef IOT_NODE_SDS011

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Sds011 {
  void setup();
}

} // section namespace
} // project namespace

#endif
