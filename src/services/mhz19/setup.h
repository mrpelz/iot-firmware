#pragma once

#ifdef IOT_NODE_MHZ19

#include <Arduino.h>

#include "../../utils/udp/main.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode
{
  namespace Services
  {

    namespace Mhz19
    {
      void setup();
    }

  } // section namespace
} // project namespace

#endif
