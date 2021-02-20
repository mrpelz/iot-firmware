#ifndef _MCP9808_HANDLER
#define _MCP9808_HANDLER

#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_MCP9808.h"

#include "../../utils/udp/main.h"
#include "../../utils/log.h"

namespace IotNode {

UDP::RequestHandler makeMcp9808Handler();

} // project namespace

#endif
