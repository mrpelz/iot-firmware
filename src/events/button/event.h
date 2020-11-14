#ifndef _BUTTON_EVENT
#define _BUTTON_EVENT

#include <Arduino.h>
#include "../event-ids.h"
#include "../../udp-messaging.h"
#include "../../button-timing.h"
#include "../../logging.h"

void buttonEvent(UDPMessaging *udp, ButtonUpdate update);

#endif
