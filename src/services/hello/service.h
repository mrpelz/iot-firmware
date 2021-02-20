#ifndef _HELLO_SERVICE
#define _HELLO_SERVICE

#include <Arduino.h>
#include "../../utils/udp/main.h"
#include "../service-ids.h"
#include "./handler.h"

namespace IotNode {

UDP::Service helloService = {
  .serviceId = serviceIds::hello,
  .handler = helloHandler,
};

} // project namespace

#endif
