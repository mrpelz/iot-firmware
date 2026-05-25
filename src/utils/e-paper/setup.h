#pragma once

#ifdef IOT_NODE_E_PAPER

#include <Arduino.h>
#include <Wire.h>

#include "epd_driver.h"

#include "./main.h"

namespace IotNode::Utils::EPaper
{
#ifdef IOT_NODE_ESP32
  void task(void *parameter);
#endif

  void setup();
}

#endif
