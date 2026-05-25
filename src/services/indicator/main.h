#pragma once

#ifdef IOT_NODE_INDICATORS

#include <Arduino.h>

#include <pre.h>
#include <post.h>

#include "../../utils/indicator/setup.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"

#define INDICATOR_CMD_OFF 0
#define INDICATOR_CMD_ON 1
#define INDICATOR_CMD_BLINK 2

namespace IotNode::Services::Indicator
{
  Utils::UDP::Service makeService(Utils::Indicator::ClassPin *output, unsigned char index);

#ifdef IOT_NODE_SX1509
  Utils::UDP::Service makeService(Utils::Indicator::ClassExpander *output, unsigned char index);
#endif
}

#endif
