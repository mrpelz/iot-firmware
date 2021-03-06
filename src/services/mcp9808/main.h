#ifndef _MCP9808_MAIN
#define _MCP9808_MAIN

#ifdef IOT_NODE_MCP9808

#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_MCP9808.h"

#include "../../utils/log.h"
#include "../../utils/udp/main.h"

namespace IotNode {

namespace Mcp9808 {
  void initializer(TwoWire *i2c);

  void responseTask(void *parameter);

  void handler(UDP::Payload *request, UDP::RespondCallback respond);
}

} // project namespace

#endif

#endif
