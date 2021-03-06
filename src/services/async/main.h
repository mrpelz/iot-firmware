#ifndef _ASYNC_MAIN
#define _ASNYC_MAIN

#ifdef ARDUINO_ARCH_ESP32

#include <Arduino.h>

#include "../../utils/log.h"
#include "../../utils/udp/main.h"

#define ASYNC_RESPONSE_DELAY 30000

namespace IotNode {

namespace Async {
  void responseTask(void *parameter);

  void handler(UDP::Payload *request, UDP::RespondCallback respond);
}

} // project namespace

#endif

#endif
