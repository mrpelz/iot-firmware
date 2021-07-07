#pragma once

#ifdef IOT_NODE_E_PAPER

#include <Arduino.h>
#include <Wire.h>

#include "epd_driver.h"

#include "./main.h"

namespace IotNode {
namespace Utils {

namespace EPaper {
  #ifdef IOT_NODE_ESP32
    void task(void *parameter);
  #endif

  void setup();
}

} // section namespace
} // project namespace

#endif
