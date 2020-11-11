#ifndef _BUTTON_EVENT
#define _BUTTON_EVENT

#include <Arduino.h>
#include "../event-ids.h"
#include "../../udp-messaging.h"
#include "../../logging.h"

void buttonEvent(
  UDPMessaging *udp,
  uint8_t index,
  bool isPressed,
  uint32_t prevDuration
);

#endif
