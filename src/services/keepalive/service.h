#ifndef _KEEPALIVE_SERVICE
#define _KEEPALIVE_SERVICE

#include <Arduino.h>
#include "../../utils/udp/main.h"
#include "../service-ids.h"
#include "./handler.h"

namespace IotNode {

UDP::Service keepAliveService = {
  .serviceId = serviceIds::keepalive,
  .handler = keepAliveHandler,
};

} // project namespace

#endif
