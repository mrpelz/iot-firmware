#ifndef _BUTTON_EVENT
#define _BUTTON_EVENT

#include <Arduino.h>
#include "../event-ids.h"
#include "../../udp-messaging.h"
#include "../../logging.h"

void buttonEvent(
  UDPMessaging *udp,
  uint8_t index,
  bool down,
  bool downChanged,
  uint8_t repeat,
  uint8_t longpress,
  unsigned long prevDuration
);

#endif
