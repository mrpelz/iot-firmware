#ifndef _UTILS_INDICATOR_SETUP
#define _UTILS_INDICATOR_SETUP

#ifdef IOT_NODE_INDICATORS

#include <Arduino.h>

#include "./main.h"

namespace IotNode {
namespace Utils {

namespace Indicator {
  extern Class indicator0;

  #ifdef IOT_NODE_BOARD_H801
    extern Class indicator1;
  #endif

  void update();

  #ifdef IOT_NODE_ESP32
    void task(void *parameter);
  #endif

  void setup();
}

} // section namespace
} // project namespace

#endif

#endif
