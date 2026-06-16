#pragma once

#ifdef IOT_NODE_BOARD_SHELLYPLUS1

#include <Arduino.h>

#include "../../events/button/main.h"
#include "../../utils/button/main.h"
#include "../../utils/udp/setup.h"

#ifdef IOT_NODE_LOGGING
#include "../../utils/log.h"
#endif

#include "./output.h"

namespace IotNode::Setup::Button
{
  extern ::IotNode::Utils::Button::Class button0;

  void update();

  void task(void *parameter);

  void setup();
}

#endif
