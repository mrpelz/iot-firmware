#ifndef _TSL2561_SERVICE
#define _TSL2561_SERVICE

#include <Arduino.h>
#include "../../utils/udp-messaging.h"
#include "../service-ids.h"
#include "./handler.h"

auto tsl2561UpdateHandler = makeTsl2561UpdateHandler();

UDPService tsl2561Service = {
  .serviceId = serviceIds::tsl2561,
  .handler = tsl2561UpdateHandler.handler,
};

#endif
