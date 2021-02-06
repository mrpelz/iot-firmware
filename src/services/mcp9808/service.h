#ifndef _MCP9808_SERVICE
#define _MCP9808_SERVICE

#include <Arduino.h>

#include "../../utils/udp-messaging.h"
#include "../service-ids.h"
#include "./handler.h"

UDPService mcp9808Service = {
  .serviceId = serviceIds::mcp9808,
  .handler = makeMcp9808Handler(),
};

#endif
