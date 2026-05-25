#pragma once

#ifdef IOT_NODE_MHZ19

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode::Services::Mhz19
{
  void setup();
}

#endif
