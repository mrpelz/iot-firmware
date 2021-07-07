#pragma once

#ifdef IOT_NODE_VCC

#include <Arduino.h>

#include "../../utils/keepalive/main.h"
#include "../../utils/vcc.h"
#include "./main.h"

namespace IotNode {
namespace Events {

namespace VCC {
  void update();

  #ifdef IOT_NODE_ESP_NOW_NODE
    void setupEspNow();
  #endif
}

} // section namespace
} // project namespace

#endif
