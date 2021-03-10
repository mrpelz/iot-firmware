#ifndef _SERVICES_MCP9808_SETUP
#define _SERVICES_MCP9808_SETUP

#ifdef IOT_NODE_MCP9808

#include <Arduino.h>
#include <Wire.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {
namespace Services {

namespace Mcp9808 {
  void setup(Utils::UDP::Class *udp, TwoWire *i2c);
}

} // section namespace
} // project namespace

#endif

#endif
