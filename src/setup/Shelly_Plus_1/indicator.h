#pragma once

#ifdef IOT_NODE_BOARD_SHELLYPLUS1

#include <Arduino.h>

#include "../../services/indicator/main.h"
#include "../../utils/indicator/main.h"
#include "../../utils/udp/setup.h"

namespace IotNode::Setup::Indicator
{
  extern ::IotNode::Utils::Indicator::ClassPin indicator0;

  void update();

  void task(void *parameter);

  void setup();
}

#endif
