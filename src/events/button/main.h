#ifndef _EVENTS_BUTTON_MAIN
#define _EVENTS_BUTTON_MAIN

#include <Arduino.h>

#include "../../utils/button/main.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"
#include "../event-ids.h"

namespace IotNode {
namespace Events {

namespace Button {
  Utils::Button::ChangeCallback makeEvent(Utils::UDP::Class *udp, uint8_t index);
}

} // section namespace
} // project namespace

#endif
