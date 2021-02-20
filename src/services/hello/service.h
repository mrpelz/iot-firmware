#ifndef _HELLO_SERVICE
#define _HELLO_SERVICE

#include <Arduino.h>
#include "../../utils/udp.h"
#include "../service-ids.h"
#include "./handler.h"

namespace IotNode {

UDPService helloService = {
  .serviceId = serviceIds::hello,
  .handler = helloHandler,
};

} // project namespace

#endif
