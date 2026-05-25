#pragma once

#ifdef IOT_NODE_ESP32

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode::Services::Async
{
  void setup();
}

#endif
