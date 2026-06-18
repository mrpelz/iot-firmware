#include "./i2c.h"

#ifdef IOT_NODE_I2C

namespace IotNode::Utils::I2C
{
#ifdef IOT_NODE_ESP8266
  TwoWire bus = TwoWire();
#endif
#ifdef IOT_NODE_ESP32
  TwoWire bus = TwoWire(0);
#endif

  volatile bool lock = false;

#ifdef IOT_NODE_I2C_SCAN
  void scan()
  {
    vTaskDelay(pdMS_TO_TICKS(100));

    unsigned char deviceCount = 0;
    unsigned char errorCount = 0;

#ifdef IOT_NODE_LOGGING
    Log::debug("i2c.scan: start");
#endif

    for (unsigned char address = 1; address < 127; address++)
    {
      bus.beginTransmission(address);
      auto error = bus.endTransmission();

      if (error == 0)
      {
        deviceCount++;

#ifdef IOT_NODE_LOGGING
        Log::debug(fmt::format("i2c.scan.found.address: {}", address, HEX));
#endif
      }
      else if (error == 4)
      {
        errorCount++;

#ifdef IOT_NODE_LOGGING
        Log::debug(fmt::format("i2c.scan.unknown-error.address: {}", address, HEX));
#endif
      }
    }

#ifdef IOT_NODE_LOGGING
    Log::debug(fmt::format("i2c.scan.found.count: {}", deviceCount));
    Log::debug(fmt::format("i2c.scan.unknown-error.count: {}", errorCount));
    Log::debug("i2c.scan: done");
#endif
  }
#endif

  void setup()
  {
#ifdef IOT_NODE_ESP32
    bus.begin(32, 33);
#else
    bus.begin();
#endif
  }

#ifdef IOT_NODE_ESP32
  void claim()
  {
    if (!lock)
    {
      lock = true;

      return;
    }

    while (lock)
    {
      vTaskDelay(pdMS_TO_TICKS(100));
    }

    lock = true;
  }

  void unclaim()
  {
    lock = false;
  }
#endif
}

#endif
