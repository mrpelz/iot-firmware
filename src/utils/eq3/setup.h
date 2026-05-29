#pragma once

#ifdef IOT_NODE_EQ3

#include <Arduino.h>
#include <BLEDevice.h>
#include <secret.h>

#include "../../lib/eQ3/eQ3.h"
#include "./main.h"

namespace IotNode::Utils::EQ3
{
#ifdef IOT_NODE_EQ3_ENTRY_DOOR
  extern Lib::EQ3::Class entryDoorLock;
#endif

  void update();

#ifdef IOT_NODE_ESP32
  void task(void *parameter);
#endif

  void setup();
}

#endif
