#pragma once

#ifdef IOT_NODE_INPUT

#include <Arduino.h>

#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode::Events::Input
{
  void update();

#ifdef IOT_NODE_ESP32
  void task(void *parameter);
#endif

  void setup();

#ifdef IOT_NODE_ESP_NOW_NODE
  void setupEspNow();
#endif
}

#endif
