#ifndef _UTILS_INDICATOR_SETUP
#define _UTILS_INDICATOR_SETUP

#ifdef IOT_NODE_INDICATORS

#include <Arduino.h>

#include "./main.h"

namespace IotNode {
namespace Utils {

namespace Indicator {
  #if defined(IOT_NODE_BOARD_ROOM_SENSOR) || defined(IOT_NODE_BOARD_TEST_DEVICE)
    extern Class indicator0;
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
