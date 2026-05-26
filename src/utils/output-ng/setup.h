#pragma once

#ifdef IOT_NODE_INDICATORS

#include <Arduino.h>

#include "./main.h"

namespace IotNode::Utils::OutputNg
{
#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
  extern Dimmable indicator0;
  extern DimmableRGB indicator1;
  extern Buzzer buzzer;
#endif

#if defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO) || defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ETH)
  extern ESP32_WS2812 ws2812Bus0;
#endif

#if defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO) || defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ETH)
  extern DimmableRGBWS2812 indicatorRGB;
#endif

  void update();

#ifdef IOT_NODE_ESP32
  void task(void *parameter);
#endif

  void setup();
}

#endif
