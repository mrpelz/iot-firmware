#ifndef _SERVICES_MCP9808_MAIN
#define _SERVICES_MCP9808_MAIN

#ifdef IOT_NODE_MCP9808

#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_MCP9808.h"

#include "../../utils/i2c.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"

namespace IotNode {
namespace Services {

namespace Mcp9808 {
  void initializer(TwoWire *i2c);

  void responseTask(void *parameter);

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer);
}

} // section namespace
} // project namespace

#endif

#endif
