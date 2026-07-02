#pragma once

#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO

#include <Arduino.h>

#include "../../services/output-ng/main.h"
#include "../../utils/output-ng/main.h"
#include "../../utils/udp/setup.h"

namespace IotNode::Setup::OutputNg
{
  extern ESP32_WS2812 ws2812Bus0;
  extern ::IotNode::Utils::OutputNg::DimmableRGBWS2812 indicatorRGB0;
  extern ::IotNode::Utils::OutputNg::Dimmable led0;
  extern ::IotNode::Utils::OutputNg::DimmableRGB ledRGB1;
  extern ::IotNode::Utils::OutputNg::Buzzer buzzer0;

  void update();
  void task(void *parameter);
  void setup();
}

#endif
