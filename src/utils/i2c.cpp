#include "./i2c.h"

namespace IotNode {
namespace Utils {

namespace I2C {
  #ifdef IOT_NODE_ESP8266
    TwoWire bus = TwoWire();
  #endif
  #ifdef IOT_NODE_ESP32
    TwoWire bus = TwoWire(0);
  #endif

  volatile bool lock = false;

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

  void setup() {
    bus.begin(32, 33);
  }

  #ifdef IOT_NODE_ESP32
    void claim() {
      if (!lock) {
        lock = true;

        return;
      }

      while (lock) {
        vTaskDelay(I2C_LOCK_DELAY / portTICK_PERIOD_MS);
      }

      lock = true;
    }

    void unclaim() {
      lock = false;
    }
  #endif
}

} // section namespace
} // project namespace
