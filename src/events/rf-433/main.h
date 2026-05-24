#pragma once

#ifdef IOT_NODE_RF_433

#include <Arduino.h>

#include <pre.h>
#include <post.h>
#include <vector>

#include <RCSwitch.h>

#include "../../utils/log.h"
#include "../../utils/udp/setup.h"
#include "../event-ids.h"

namespace IotNode
{
  namespace Events
  {

    namespace Rf433
    {
      void event(std::vector<uint8_t> payload);

      void onDataReceived(unsigned int *protocol, unsigned int *bitLength, unsigned long *value);
    }

  } // section namespace
} // project namespace

#endif
