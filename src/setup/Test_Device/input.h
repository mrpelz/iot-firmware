#pragma once

#ifdef IOT_NODE_BOARD_TEST_DEVICE

#include <Arduino.h>

#include "../../utils/udp/setup.h"
#include "../../events/input/main.h"

namespace IotNode::Setup::Input
{
  void update();

  void task(void *parameter);

  void setup();
}

#endif
