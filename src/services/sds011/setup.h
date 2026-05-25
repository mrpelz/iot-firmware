#pragma once

#ifdef IOT_NODE_SDS011

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode::Services::Sds011
{
  void setup();
}

#endif
