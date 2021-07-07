#pragma once

#ifdef IOT_NODE_BUTTONS

#include <Arduino.h>

#include "../../utils/button/main.h"
#include "../../utils/esp-now-node.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"
#include "../event-ids.h"

namespace IotNode {
namespace Events {

namespace Button {
  Utils::Button::ChangeCallback makeEvent(Utils::UDP::Class *udp, uint8_t index);

  #ifdef IOT_NODE_ESP_NOW_NODE
    Utils::Button::ChangeCallback makeEspNowEvent(uint8_t index);
  #endif
}

} // section namespace
} // project namespace

#endif
