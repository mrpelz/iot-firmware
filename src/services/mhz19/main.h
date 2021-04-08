#ifndef _SERVICES_MHZ19_MAIN
#define _SERVICES_MHZ19_MAIN

#ifdef IOT_NODE_MHZ19

#include <Arduino.h>

#include "MHZ19.h"

#include "../../utils/log.h"
#include "../../utils/udp/main.h"

namespace IotNode {
namespace Services {

namespace Mhz19 {
  void initializer();

  void responseTask(void *parameter);

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer);
}

} // section namespace
} // project namespace

#endif

#endif
