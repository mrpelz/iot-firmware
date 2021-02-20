#ifndef _MCP9808_HANDLER
#define _MCP9808_HANDLER

#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_MCP9808.h"

#include "../../utils/udp-messaging.h"
#include "../../utils/log.h"

RequestHandler makeMcp9808Handler();

#endif
