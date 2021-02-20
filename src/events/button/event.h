#ifndef _BUTTON_EVENT
#define _BUTTON_EVENT

#include <Arduino.h>
#include "./handler.h"
#include "../event-ids.h"
#include "../../utils/udp-messaging.h"
#include "../../utils/log.h"

void buttonEvent(UDPMessaging *udp, ButtonUpdate update);

#endif
