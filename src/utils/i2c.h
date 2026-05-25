#pragma once

#ifdef IOT_NODE_I2C

#include <Arduino.h>
#include <Wire.h>

#include "./utils/log.h"

#define I2C_LOCK_DELAY 10
#define I2C_START_DELAY 10000

namespace IotNode::Utils::I2C
{
  extern TwoWire bus;

#ifdef IOT_NODE_I2C_SCAN
  void scan();
#endif

  void setup();

#ifdef IOT_NODE_ESP32
  void claim();

  void unclaim();
#endif
}

#endif
