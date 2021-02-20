#ifndef _ASYNC_SERVICE
#define _ASYNC_SERVICE

#include <Arduino.h>
#include "../../utils/udp.h"
#include "../service-ids.h"
#include "./handler.h"

namespace IotNode {

UDPService asyncService = {
  .serviceId = serviceIds::async,
  .handler = asyncHandler,
};

} // project namespace

#endif
