#pragma once

#ifdef IOT_NODE_VCC

#include <Arduino.h>

#include <pre.h>
#include <post.h>
#include <functional>

#include "../../utils/esp-now-node.h"
#include "../../utils/log.h"
#include "../event-ids.h"

namespace IotNode::Events::VCC
{
  typedef ::std::function<void(unsigned short vcc)> ChangeCallback;

#ifdef IOT_NODE_ESP_NOW_NODE
  ChangeCallback makeEspNowEvent();
#endif
}

#endif
