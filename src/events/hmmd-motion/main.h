#pragma once

#ifdef IOT_NODE_HMMD_MOTION

#include <Arduino.h>
#include <vector>

#include "../../utils/log.h"
#include "../../utils/udp/setup.h"
#include "../event-ids.h"
#include "./sensor.h"

namespace IotNode
{
  namespace Events
  {

    namespace HmmdMotion
    {
      void initializer();

      void event(bool isTargetDetected, int16_t distance);

      void checkForTargets();
    }

  } // section namespace
} // project namespace

#endif
