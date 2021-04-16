#ifndef _EVENTS_BUTTON_SETUP
#define _EVENTS_BUTTON_SETUP

#ifdef IOT_NODE_BUTTONS

#include <Arduino.h>

#include "../../utils/button/setup.h"
#include "../../utils/log.h"
#include "../../utils/relais/setup.h"
#include "../../utils/udp/setup.h"
#include "./main.h"

#ifdef IOT_NODE_ESP_NOW_NODE
  #include "../../utils/button/main.h"
#endif

namespace IotNode {
namespace Events {

namespace Button {
  void setup();

  #ifdef IOT_NODE_ESP_NOW_NODE
    void setupEspNow();
  #endif
}

} // section namespace
} // project namespace

#endif

#endif
