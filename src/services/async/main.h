#ifndef _SERVICES_ASYNC_MAIN
#define _SERVICES_ASNYC_MAIN

#ifdef IOT_NODE_ESP32

#include <Arduino.h>

#include "../../utils/log.h"
#include "../../utils/udp/main.h"

#define ASYNC_RESPONSE_DELAY 30000

namespace IotNode {
namespace Services {

namespace Async {
  void initializer();

  void responseTask(void *parameter);

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer);
}

} // section namespace
} // project namespace

#endif

#endif
