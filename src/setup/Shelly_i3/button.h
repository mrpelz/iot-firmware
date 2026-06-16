#pragma once

#ifdef IOT_NODE_BOARD_SHELLYI3

#include <Arduino.h>

#include "../../events/button/main.h"
#include "../../utils/button/main.h"
#include "../../utils/udp/setup.h"

namespace IotNode::Setup::Button
{
  extern ::IotNode::Utils::Button::Class button0;
  extern ::IotNode::Utils::Button::Class button1;
  extern ::IotNode::Utils::Button::Class button2;

  void update();

  void setup();
}

#endif
