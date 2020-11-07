#ifndef _KEEPALIVE_SERVICE
#define _KEEPALIVE_SERVICE

#include <Arduino.h>
#include "../../udp-messaging.h"
#include "../service-ids.h"
#include "./handler.h"

UDPService keepAliveService = {
  serviceIds::keepalive,
  keepAliveHandler
};

#endif
