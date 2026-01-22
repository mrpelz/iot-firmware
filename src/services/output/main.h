#pragma once

#ifdef IOT_NODE_OUTPUT

#include "../../utils/output/main.h"
#include "../../utils/udp/main.h"
#include "../service-ids.h"

namespace IotNode
{
  namespace Services
  {

    namespace Output
    {
      Utils::UDP::Service makeService(Utils::Output::Regular *output, uint8_t index);
      Utils::UDP::Service makeService(Utils::Output::Pulse *output, uint8_t index);
    }

  } // section namespace
} // project namespace

#endif
