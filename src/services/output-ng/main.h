#pragma once

#ifdef IOT_NODE_OUTPUT_NG

#include <Arduino.h>

#include <pre.h>
#include <post.h>

#include "../../utils/output-ng/main.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"

#define BUZZER_ITEM_SIZE 12
#define BINARY_ITEM_SIZE 5
#define DIMMABLE_ITEM_SIZE 16
#define DIMMABLERGB_ITEM_SIZE 32

namespace IotNode::Services::OutputNg
{
  Utils::UDP::Service makeService(Utils::OutputNg::Buzzer *instance, Utils::UDP::Class *udp, unsigned char index);
  Utils::UDP::Service makeService(Utils::OutputNg::Binary *instance, Utils::UDP::Class *udp, unsigned char index);
  Utils::UDP::Service makeService(Utils::OutputNg::Dimmable *instance, Utils::UDP::Class *udp, unsigned char index);
  Utils::UDP::Service makeService(Utils::OutputNg::DimmableRGB *instance, Utils::UDP::Class *udp, unsigned char index);
}

#endif
