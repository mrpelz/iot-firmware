#ifndef _UTILS_OUTPUT_SETUP
#define _UTILS_OUTPUT_SETUP

#ifdef IOT_NODE_OUTPUT

#include <Arduino.h>

#include "./main.h"

namespace IotNode {
namespace Utils {

namespace Output {
  #ifdef IOT_NODE_BOARD_SHELLY1
    extern Regular output0;
  #elif defined(IOT_NODE_BOARD_OBI_JACK)
    extern Pulse output0;
  #endif

  void setup();
}

} // section namespace
} // project namespace

#endif

#endif
