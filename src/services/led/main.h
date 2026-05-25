#pragma once

#ifdef IOT_NODE_LED

#include <FadeLed.h>

#include "../../utils/udp/main.h"
#include "../service-ids.h"

namespace IotNode::Services::Led
{
  Utils::UDP::Service makeService(FadeLed *led, uint8_t index);
}

#endif
