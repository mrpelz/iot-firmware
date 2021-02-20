#ifndef _BUTTON_EVENT
#define _BUTTON_EVENT

#include <Arduino.h>

#include "../../utils/log.h"
#include "../../utils/udp/main.h"
#include "../event-ids.h"
#include "./main.h"

namespace IotNode {

namespace Button {
  void buttonEvent(UDP::Class *udp, Update update);
}

} // project namespace

#endif
