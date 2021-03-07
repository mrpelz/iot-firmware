#ifndef _I2C
#define _I2C

#include <Arduino.h>
#include <Wire.h>

#include "./utils/log.h"

namespace IotNode {

namespace I2C {
  #ifdef IOT_NODE_I2C_SCAN
    void scan();
  #endif

  TwoWire *setup();
}

} // project namespace

#endif
