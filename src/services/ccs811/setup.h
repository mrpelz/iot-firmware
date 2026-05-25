#pragma once

#ifdef IOT_NODE_CCS811

#include <Arduino.h>

#include "../../utils/i2c.h"
#include "../../utils/udp/main.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode::Services::Ccs811
{
  void setup();
}

#endif
