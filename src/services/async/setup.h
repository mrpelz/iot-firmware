#pragma once

#ifdef IOT_NODE_ESP32

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Async {
  void setup();
}

} // section namespace
} // project namespace

#endif
