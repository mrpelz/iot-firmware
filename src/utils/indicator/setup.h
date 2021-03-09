#ifndef _INDICATOR_SETUP
#define _INDICATOR_SETUP

#include <Arduino.h>

#include "./main.h"

namespace IotNode {

namespace Indicator {
  extern Class rxdLed;
  extern Class txdLed;

  void update();

  #ifdef ARDUINO_ARCH_ESP32
    void task(void *parameter);
  #endif

  void setup();
}

} // project namespace

#endif
