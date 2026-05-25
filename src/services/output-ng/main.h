#pragma once

#if defined(IOT_NODE_INDICATORS) && defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO)

#include <Arduino.h>

#include <pre.h>
#include <post.h>

#include "../../utils/output-ng/setup.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"

#define BUZZER_ITEM_SIZE 12
#define BINARY_ITEM_SIZE 5
#define DIMMABLE_ITEM_SIZE 16
#define DIMMABLERGB_ITEM_SIZE 32

namespace IotNode::Services::OutputNg
{
  Utils::UDP::Service makeService(Utils::OutputNg::Buzzer *instance, unsigned char index);
  Utils::UDP::Service makeService(Utils::OutputNg::Binary *instance, unsigned char index);
  Utils::UDP::Service makeService(Utils::OutputNg::Dimmable *instance, unsigned char index);
  Utils::UDP::Service makeService(Utils::OutputNg::DimmableRGB *instance, unsigned char index);
}

#endif
