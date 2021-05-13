#ifndef _EVENTS_VCC_MAIN
#define _EVENTS_VCC_MAIN

#ifdef IOT_NODE_VCC

#include <Arduino.h>
#include <functional>

#include "../../utils/esp-now-node.h"
#include "../../utils/log.h"
#include "../event-ids.h"

namespace IotNode {
namespace Events {

namespace VCC {
  typedef std::function<void (uint16_t vcc)> ChangeCallback;

  #ifdef IOT_NODE_ESP_NOW_NODE
    ChangeCallback makeEspNowEvent();
  #endif
}

} // section namespace
} // project namespace

#endif

#endif
