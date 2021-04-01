#ifndef _UTILS_INDICATOR_SETUP
#define _UTILS_INDICATOR_SETUP

#include <Arduino.h>

#include "./main.h"

namespace IotNode {
namespace Utils {

namespace Indicator {
  extern Class rxdLed;
  extern Class txdLed;

  void update();

  #ifdef IOT_NODE_ESP32
    void task(void *parameter);
  #endif

  void setup();
}

} // section namespace
} // project namespace

#endif
