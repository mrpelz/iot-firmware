#pragma once

#if defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V1) || defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V13)

#include <Arduino.h>

#include "../../events/button/main.h"
#include "../../utils/button/main.h"
#include "../../utils/keepalive/setup.h"
#include "../../utils/udp/setup.h"

#ifdef IOT_NODE_LOGGING
#include "../../utils/log.h"
#endif

#include "./output.h"

namespace IotNode::Setup::Button
{
  extern ::IotNode::Utils::Button::Class button0;

  void update();

  void setup();
}

#endif
