#pragma once

#ifdef IOT_NODE_BOARD_H801

#include <Arduino.h>

#include "../../services/indicator/main.h"
#include "../../utils/indicator/main.h"
#include "../../utils/udp/setup.h"

namespace IotNode::Setup::Indicator
{
  extern ::IotNode::Utils::Indicator::ClassPin indicator0;
  extern ::IotNode::Utils::Indicator::ClassPin indicator1;

  void update();

  void setup();
}

#endif
