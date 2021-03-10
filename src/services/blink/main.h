#ifndef _BLINK_MAIN
#define _BLINK_MAIN

#include <Arduino.h>

#include "../../utils/indicator/setup.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"

namespace IotNode {

namespace Blink {
  void handler(UDP::Payload *request, UDP::RespondCallback respond);
}

} // project namespace

#endif
