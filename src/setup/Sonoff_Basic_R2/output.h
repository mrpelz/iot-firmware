#pragma once

#if defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V1) || defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V13)

#include <Arduino.h>

#include "../../services/output/main.h"
#include "../../utils/output/main.h"
#include "../../utils/udp/setup.h"

namespace IotNode::Setup::Output
{
  extern ::IotNode::Utils::Output::Regular output0;

  void setup();
}

#endif
