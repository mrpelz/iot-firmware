#ifndef _HELLO_SERVICE
#define _HELLO_SERVICE

#include <Arduino.h>
#include "../../utils/udp-messaging.h"
#include "../service-ids.h"
#include "./handler.h"

UDPService helloService = {
  .serviceId = serviceIds::hello,
  .handler = helloHandler,
};

#endif
