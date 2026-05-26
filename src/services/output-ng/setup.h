#pragma once

#if defined(IOT_NODE_INDICATORS) && (defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO) || defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ETH))

#include <Arduino.h>

#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode::Services::OutputNg
{
  void setup();
}

#endif
