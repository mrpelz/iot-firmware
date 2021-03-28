#ifndef _UTILS_I2C
#define _UTILS_I2C

#include <Arduino.h>
#include <Wire.h>

#include "./utils/log.h"

#define I2C_LOCK_DELAY 10

namespace IotNode {
namespace Utils {

namespace I2C {
  extern TwoWire bus;

  #ifdef IOT_NODE_I2C_SCAN
    void scan();
  #endif

  void setup();

  void claim();

  void unclaim();
}

} // section namespace
} // project namespace

#endif
