#pragma once

#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ETH

#include <Arduino.h>

#include "../../services/output-ng/main.h"
#include "../../utils/output-ng/main.h"
#include "../../utils/udp/setup.h"

namespace IotNode::Setup::OutputNg
{
  extern ::IotNode::Utils::OutputNg::DimmableRGBWS2812 indicatorRGB;
  extern ESP32_WS2812 ws2812Bus0;

  void update();

  void task(void *parameter);

  void setup();
}

#endif
