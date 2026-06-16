#pragma once

#ifdef IOT_NODE_BOARD_ROOM_SENSOR

#include <Arduino.h>

#include "../../services/indicator/main.h"
#include "../../utils/indicator/main.h"
#include "../../utils/sx1509/setup.h"
#include "../../utils/udp/setup.h"

namespace IotNode::Setup::Indicator
{
  extern ::IotNode::Utils::Indicator::ClassExpander indicator0;

  void update();
  void setup();
}

#endif
