#include "./i2c.h"

namespace IotNode {

namespace I2C {
  #ifdef ARDUINO_ARCH_ESP8266
    auto bus = TwoWire();
  #endif
  #ifdef ARDUINO_ARCH_ESP32
    auto bus = TwoWire(0);
  #endif

  TwoWire *setup() {
    bus.begin(14, 15);

    return &bus;
  }
}

} // project namespace
