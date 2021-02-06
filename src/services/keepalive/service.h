#ifndef _KEEPALIVE_SERVICE
#define _KEEPALIVE_SERVICE

#include <Arduino.h>
#include "../../utils/udp-messaging.h"
#include "../service-ids.h"
#include "./handler.h"

UDPService keepAliveService = {
  .serviceId = serviceIds::keepalive,
  .handler = keepAliveHandler,
};

#endif
