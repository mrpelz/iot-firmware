#pragma once

#ifdef IOT_NODE_LED

#include <Arduino.h>

#include "../../utils/led/setup.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode::Services::Led
{
  void setup();
}

#endif
