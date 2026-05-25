#pragma once

#ifdef IOT_NODE_OUTPUT

#include "../../utils/output/main.h"
#include "../../utils/udp/main.h"
#include "../service-ids.h"

namespace IotNode::Services::Output
{
  Utils::UDP::Service makeService(Utils::Output::Regular *output, unsigned char index);
  Utils::UDP::Service makeService(Utils::Output::Pulse *output, unsigned char index);
}

#endif
