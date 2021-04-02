#ifndef _SERVICES_RELAIS_MAIN
#define _SERVICES_RELAIS_MAIN

#ifdef IOT_NODE_RELAIS

#include "../../utils/relais/main.h"
#include "../../utils/udp/main.h"
#include "../service-ids.h"

namespace IotNode {
namespace Services {

namespace Relais {
  Utils::UDP::Service makeService(Utils::Relais::Class *relais, uint8_t index);
}

} // section namespace
} // project namespace

#endif

#endif
