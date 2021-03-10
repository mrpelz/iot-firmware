#ifndef _EVENTS_BUTTON_EVENT
#define _EVENTS_BUTTON_EVENT

#include <Arduino.h>

#include "../../utils/log.h"
#include "../../utils/udp/main.h"
#include "../event-ids.h"
#include "./main.h"

namespace IotNode {
namespace Events {

namespace Button {
  void buttonEvent(Utils::UDP::Class *udp, Update update);
}

} // section namespace
} // project namespace

#endif
