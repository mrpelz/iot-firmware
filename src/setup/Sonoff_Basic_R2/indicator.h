#pragma once

#if defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V1) || defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V13)

#include <Arduino.h>

#include "../../services/indicator/main.h"
#include "../../utils/indicator/main.h"
#include "../../utils/udp/setup.h"

namespace IotNode::Setup::Indicator
{
  extern ::IotNode::Utils::Indicator::ClassPin indicator0;

  void update();

  void setup();
}

#endif
