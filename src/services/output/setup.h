#pragma once

#ifdef IOT_NODE_OUTPUT

#include <Arduino.h>

#include "../../utils/output/setup.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode::Services::Output
{
  void setup();
}

#endif
