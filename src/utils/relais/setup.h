#ifndef _UTILS_RELAIS_SETUP
#define _UTILS_RELAIS_SETUP

#ifdef IOT_NODE_RELAIS

#include <Arduino.h>

#include "./main.h"

namespace IotNode {
namespace Utils {

namespace Relais {
  #ifdef IOT_NODE_BOARD_SHELLY1
    extern Class relais0;
  #endif

  void setup();
}

} // section namespace
} // project namespace

#endif

#endif
