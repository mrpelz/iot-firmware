#pragma once

#ifdef IOT_NODE_HMMD_MOTION

#include <Arduino.h>

#include "../../utils/log.h"
#include "./main.h"

namespace IotNode::Events::HmmdMotion
{
  void update();
  void setup();
}

#endif
