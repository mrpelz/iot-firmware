#ifndef _UTILS_LED_SETUP
#define _UTILS_LED_SETUP

#ifdef IOT_NODE_LED

#include <Arduino.h>

#include "./main.h"

namespace IotNode {
namespace Utils {

namespace Led {
  #ifdef IOT_NODE_BOARD_H801
    extern Class led0;
    extern Class led1;
    extern Class led2;
    extern Class led3;
    extern Class led4;
  #endif

  void setup();
}

} // section namespace
} // project namespace

#endif

#endif
