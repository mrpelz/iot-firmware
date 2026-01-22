#pragma once

#ifdef IOT_NODE_TSL2561

#include <Arduino.h>

#include "../../utils/i2c.h"
#include "../../utils/udp/main.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode
{
  namespace Services
  {

    namespace Tsl2561
    {
      void setup();
    }

  } // section namespace
} // project namespace

#endif
