#pragma once

#ifdef IOT_NODE_HMMD_MOTION

#include <Arduino.h>

#include <pre.h>
#include <post.h>
#include <vector>

#include "../../utils/log.h"
#include "../../utils/udp/setup.h"
#include "../event-ids.h"
#include "./sensor.h"

namespace IotNode::Events::HmmdMotion
{
  void initializer();

  void event(bool isTargetDetected, int16_t distance);

  void checkForTargets();
}

#endif
