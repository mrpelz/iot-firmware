#ifndef _HELLO_SERVICE
#define _HELLO_SERVICE

#include <Arduino.h>
#include "../../udp-messaging.h"
#include "../service-ids.h"
#include "./handler.h"

UDPService helloService = {
  serviceIds::hello,
  helloHandler
};

#endif
