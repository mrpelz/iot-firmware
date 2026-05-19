#pragma once

#ifdef IOT_NODE_INDICATORS

#include <Arduino.h>

#include "./main.h"

namespace IotNode
{
  namespace Utils
  {

    namespace Indicator
    {
#if defined(IOT_NODE_BOARD_ROOM_SENSOR)
      extern ClassExpander indicator0;
      extern ClassExpander indicator1;
      extern ClassExpander indicator2;
#elif !defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO)
      extern ClassPin indicator0;
#endif

#ifdef IOT_NODE_BOARD_H801
      extern ClassPin indicator1;
#endif

      void update();

#ifdef IOT_NODE_ESP32
      void task(void *parameter);
#endif

      void setup();
    }

  } // section namespace
} // project namespace

#endif
