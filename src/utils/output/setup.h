#pragma once

#ifdef IOT_NODE_OUTPUT

#include <Arduino.h>

#include "./main.h"

namespace IotNode {
namespace Utils {

namespace Output {
  #if defined(IOT_NODE_BOARD_SHELLY1) || defined(IOT_NODE_BOARD_SONOFF_BASIC)
    extern Regular output0;
  #elif defined(IOT_NODE_BOARD_OBI_JACK)
    extern Pulse output0;
  #endif

  void setup();
}

} // section namespace
} // project namespace

#endif
