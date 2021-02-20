#ifndef _ASYNC_SERVICE
#define _ASYNC_SERVICE

#include <Arduino.h>
#include "../../utils/udp/main.h"
#include "../service-ids.h"
#include "./handler.h"

namespace IotNode {

UDP::Service asyncService = {
  .serviceId = serviceIds::async,
  .handler = asyncHandler,
};

} // project namespace

#endif
