#pragma once

#ifdef IOT_NODE_INDICATORS

#include <Arduino.h>

#include "../../utils/indicator/setup.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"

#define INDICATOR_CMD_OFF 0
#define INDICATOR_CMD_ON 1
#define INDICATOR_CMD_BLINK 2

namespace IotNode
{
  namespace Services
  {

    namespace Indicator
    {
      Utils::UDP::Service makeService(Utils::Indicator::ClassPin *output, uint8_t index);

#ifdef IOT_NODE_SX1509
      Utils::UDP::Service makeService(Utils::Indicator::ClassExpander *output, uint8_t index);
#endif

#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
      Utils::UDP::Service makeService(Utils::Indicator::ClassWS2812 *output, uint8_t index);
#endif
    }

  } // section namespace
} // project namespace

#endif
