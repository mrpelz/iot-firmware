#pragma once

#ifdef IOT_NODE_BOARD_SHELLYPLUS1

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
