#pragma once

#ifdef IOT_NODE_RF_433

#include <Arduino.h>

#include "../../utils/log.h"
#include "./main.h"

namespace IotNode::Events::Rf433
{
  void update();

#ifdef IOT_NODE_ESP32
  void task(void *parameter);
#endif

  void setup();
}

#endif
