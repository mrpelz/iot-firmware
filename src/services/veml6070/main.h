#pragma once

#ifdef IOT_NODE_VEML6070

#include <Arduino.h>

#include <pre.h>
#include <post.h>
#include <Wire.h>

#include "../../utils/i2c.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"
#include "./sensor.h"

namespace IotNode
{
  namespace Services
  {

    namespace Veml6070
    {
      void initializer(TwoWire *i2c);

      void responseTask(void *parameter);

      void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer);
    }

  } // section namespace
} // project namespace

#endif
