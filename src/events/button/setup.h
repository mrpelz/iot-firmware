#pragma once

#ifdef IOT_NODE_BUTTONS

#include <Arduino.h>

#include "../../utils/button/setup.h"
#include "../../utils/keepalive/setup.h"
#include "../../utils/log.h"
#include "../../utils/output/setup.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode::Events::Button
{
  void setup();

#ifdef IOT_NODE_ESP_NOW_NODE
  void setupEspNow();
#endif
}

#endif
