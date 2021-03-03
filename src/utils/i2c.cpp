#include "./i2c.h"

namespace IotNode {

namespace I2C {
  auto bus = TwoWire(0);

  TwoWire *setup() {
    bus.begin(14, 15);

    return &bus;
  }
}

} // project namespace
