#ifndef _SYSTEM_INFO_SERVICE
#define _SYSTEM_INFO_SERVICE

#include <Arduino.h>
#include "../../utils/udp/main.h"
#include "../service-ids.h"
#include "./handler.h"

namespace IotNode {

UDP::Service systemInfoService = {
  .serviceId = serviceIds::systemInfo,
  .handler = systemInfoHandler,
};

} // project namespace

#endif
