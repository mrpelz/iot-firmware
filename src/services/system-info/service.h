#ifndef _SYSTEM_INFO_SERVICE
#define _SYSTEM_INFO_SERVICE

#include <Arduino.h>
#include "../../udp-messaging.h"
#include "../service-ids.h"
#include "./handler.h"

UDPService systemInfoService = {
  .serviceId = serviceIds::systemInfo,
  .handler = systemInfoHandler,
};

#endif
