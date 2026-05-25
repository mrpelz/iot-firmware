#pragma once

#ifdef IOT_NODE_BUTTONS

#include <Arduino.h>

#include <pre.h>
#include <post.h>

#include "../../utils/button/main.h"
#include "../../utils/esp-now-node.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"
#include "../event-ids.h"

namespace IotNode::Events::Button
{
  Utils::Button::ChangeCallback makeEvent(Utils::UDP::Class *udp, unsigned char index);

#ifdef IOT_NODE_ESP_NOW_NODE
  Utils::Button::ChangeCallback makeEspNowEvent(unsigned char index);
#endif
}

#endif
