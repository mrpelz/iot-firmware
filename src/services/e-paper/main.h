#pragma once

#ifdef IOT_NODE_E_PAPER

#include "../../utils/udp/main.h"
#include "../service-ids.h"

namespace IotNode {
namespace Services {

namespace EPaper {
  Utils::UDP::RequestHandler makeHandler();
}

} // section namespace
} // project namespace

#endif
