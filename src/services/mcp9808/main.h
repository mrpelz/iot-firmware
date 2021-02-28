#ifndef _MCP9808_MAIN
#define _MCP9808_MAIN

#include <Arduino.h>

#include "Adafruit_MCP9808.h"

#include "../../utils/log.h"
#include "../../utils/udp/main.h"

namespace IotNode {

namespace Mcp9808 {
  void initializer();

  void responseTask(void * parameter);

  void handler(UDP::Payload *request, UDP::RespondCallback respond);
}

} // project namespace

#endif
