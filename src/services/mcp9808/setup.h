#ifndef _MCP9808_SETUP
#define _MCP9808_SETUP

#ifdef IOT_NODE_MCP9808

#include <Arduino.h>
#include <Wire.h>

#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {

namespace Mcp9808 {
  void setup(UDP::Class *udp, TwoWire *i2c);
}

} // project namespace

#endif

#endif
