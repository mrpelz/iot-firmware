#ifndef _KEEPALIVE_SERVICE
#define _KEEPALIVE_SERVICE

#include <Arduino.h>
#include "../../utils/udp.h"
#include "../service-ids.h"
#include "./handler.h"

namespace IotNode {

UDPService keepAliveService = {
  .serviceId = serviceIds::keepalive,
  .handler = keepAliveHandler,
};

} // project namespace

#endif
