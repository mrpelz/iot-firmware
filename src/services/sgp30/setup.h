#pragma once

#ifdef IOT_NODE_SGP30

#include <Arduino.h>

#include "../../utils/i2c.h"
#include "../../utils/udp/main.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode::Services::Sgp30
{
  void setup();
}

#endif
