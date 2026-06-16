#pragma once

#if defined(IOT_NODE_BOARD_SHELLY1) && defined(IOT_NODE_INPUT)

#include <Arduino.h>

#include "../../events/input/main.h"
#include "../../utils/udp/setup.h"

namespace IotNode::Setup::Input
{
  void update();

  void setup();
}

#endif
