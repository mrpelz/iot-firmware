#pragma once

#ifdef IOT_NODE_EQ3

#include <Arduino.h>

#include "./main.h"

namespace IotNode::Utils::EQ3
{
  void update();

#ifdef IOT_NODE_ESP32
  void task(void *parameter);
#endif

  void setup();
}

#endif
