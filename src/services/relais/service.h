#ifndef _RELAIS_SERVICE
#define _RELAIS_SERVICE

#include <Arduino.h>
#include "../../utils/udp-messaging.h"
#include "../service-ids.h"
#include "./handler.h"

UDPService makeRelaisService(Relais *relais, uint8_t index);

#endif
