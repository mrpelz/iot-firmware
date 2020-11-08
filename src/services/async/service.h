#ifndef _ASYNC_SERVICE
#define _ASYNC_SERVICE

#include <Arduino.h>
#include "../../udp-messaging.h"
#include "../service-ids.h"
#include "./handler.h"

UDPService asyncService = {
  serviceIds::async,
  asyncHandler
};

#endif
