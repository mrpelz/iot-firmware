#ifndef _RELAIS_SERVICE
#define _RELAIS_SERVICE

#include <Arduino.h>
#include "../../udp-messaging.h"
#include "../service-ids.h"
#include "./handler.h"

UDPService makeRelaisService(uint8_t index, uint8_t pin, bool invert);

#endif
