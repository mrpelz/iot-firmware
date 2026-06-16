#pragma once

#ifdef IOT_NODE_BOARD_OBI_JACK

#include <Arduino.h>

#include "../../events/button/main.h"
#include "../../utils/button/main.h"
#include "../../utils/udp/setup.h"

#include "./output.h"

namespace IotNode::Setup::Button
{
  extern ::IotNode::Utils::Button::Class button0;

  void update();

  void setup();
}

#endif
