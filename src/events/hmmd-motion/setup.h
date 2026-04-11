#pragma once

#ifdef IOT_NODE_HMMD_MOTION

#include <Arduino.h>

#include "../../utils/log.h"
#include "./main.h"

namespace IotNode
{
  namespace Events
  {

    namespace HmmdMotion
    {
      void update();
      void setup();
    }

  } // section namespace
} // project namespace

#endif
