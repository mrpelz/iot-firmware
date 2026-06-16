#pragma once

#ifdef IOT_NODE_BOARD_OBI_JACK

#include <Arduino.h>

#include "../../services/output/main.h"
#include "../../utils/output/main.h"
#include "../../utils/udp/setup.h"

namespace IotNode::Setup::Output
{
  extern ::IotNode::Utils::Output::Pulse output0;

  void setup();
}

#endif
