#pragma once

#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO

#include <Arduino.h>

#include "../../services/output-ng/main.h"
#include "../../utils/output-ng/main.h"
#include "../../utils/udp/setup.h"

namespace IotNode::Setup::OutputNg
{
  extern ::IotNode::Utils::OutputNg::Dimmable indicator0;
  extern ::IotNode::Utils::OutputNg::DimmableRGB indicator1;
  extern ::IotNode::Utils::OutputNg::Buzzer buzzer;
  extern ESP32_WS2812 ws2812Bus0;
  extern ::IotNode::Utils::OutputNg::DimmableRGBWS2812 indicatorRGB;

  void update();
  void task(void *parameter);
  void setup();
}

#endif
