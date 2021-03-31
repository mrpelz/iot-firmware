#ifndef _SERVICES_ASYNC_MAIN
#define _SERVICES_ASNYC_MAIN

#ifdef ARDUINO_ARCH_ESP32

#include <Arduino.h>

#include "../../utils/log.h"
#include "../../utils/udp/main.h"

#define ASYNC_RESPONSE_DELAY 30000

namespace IotNode {
namespace Services {

namespace Async {
  void initializer();

  void responseTask(void *parameter);

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond);
}

} // section namespace
} // project namespace

#endif

#endif
