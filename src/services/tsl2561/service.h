#ifndef _TSL2561_SERVICE
#define _TSL2561_SERVICE

#include <Arduino.h>
#include "../../utils/udp/main.h"
#include "../service-ids.h"
#include "./handler.h"

namespace IotNode {

auto tsl2561UpdateHandler = makeTsl2561UpdateHandler();

UDP::Service tsl2561Service = {
  .serviceId = serviceIds::tsl2561,
  .handler = tsl2561UpdateHandler.handler,
};

} // project namespace

#endif
