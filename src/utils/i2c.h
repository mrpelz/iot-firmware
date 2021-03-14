#ifndef _UTILS_I2C
#define _UTILS_I2C

#include <Arduino.h>
#include <Wire.h>

#include "./utils/log.h"

namespace IotNode {
namespace Utils {

namespace I2C {
  extern TwoWire bus;

  #ifdef IOT_NODE_I2C_SCAN
    void scan();
  #endif

  void setup();
}

} // section namespace
} // project namespace

#endif
