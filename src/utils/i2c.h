#ifndef _I2C
#define _I2C

#include <Arduino.h>
#include <Wire.h>

namespace IotNode {

namespace I2C {
  TwoWire *setup();
}

} // project namespace

#endif
