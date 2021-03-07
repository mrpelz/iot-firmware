#include "./i2c.h"

namespace IotNode {

namespace I2C {
  #ifdef ARDUINO_ARCH_ESP8266
    auto bus = TwoWire();
  #endif
  #ifdef ARDUINO_ARCH_ESP32
    auto bus = TwoWire(0);
  #endif

  #ifdef IOT_NODE_I2C_SCAN
    void scan() {
      uint8_t deviceCount = 0;
      uint8_t errorCount = 0;

      Log::debug("i2c.scan", "start");

      for(uint8_t address = 1; address < 127; address++) {
        bus.beginTransmission(address);
        auto error = bus.endTransmission();
    
        if (error == 0) {
          deviceCount++;
          Log::debug("i2c.scan.found.address", String(address, HEX));
        } else if (error == 4) {
          errorCount++;
          Log::debug("i2c.scan.unknown-error.address", String(address, HEX));
        }
      }

      Log::debug("i2c.scan.found.count", String(deviceCount));
      Log::debug("i2c.scan.unknown-error.count", String(errorCount));
      Log::debug("i2c.scan", "done");
    }
  #endif

  TwoWire *setup() {
    bus.begin(14, 15);

    return &bus;
  }
}

} // project namespace
