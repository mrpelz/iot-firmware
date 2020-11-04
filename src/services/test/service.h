#ifndef _TEST_SERVICE
#define _TEST_SERVICE

#include <Arduino.h>
#include "../../udp-messaging.h"
#include "./handler.h"

UDPService testService = {
  0x01,
  handler
};

#endif
