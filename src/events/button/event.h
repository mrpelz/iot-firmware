#ifndef _BUTTON_EVENT
#define _BUTTON_EVENT

#include <Arduino.h>
#include "./handler.h"
#include "../event-ids.h"
#include "../../utils/udp/main.h"
#include "../../utils/log.h"

namespace IotNode {

void buttonEvent(UDP::Class *udp, ButtonUpdate update);

} // project namespace

#endif
