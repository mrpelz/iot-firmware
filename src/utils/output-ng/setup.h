#pragma once

#ifdef IOT_NODE_INDICATORS

#include <Arduino.h>

#include "./main.h"

namespace IotNode
{
  namespace Utils
  {

    namespace OutputNg
    {
      extern Dimmable indicator0;
      extern DimmableRGB indicator1;
      extern Buzzer buzzer;

#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
      extern ESP32_WS2812 ws2812Bus0;
#endif

#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
      extern DimmableRGBWS2812 indicatorRGB;
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
