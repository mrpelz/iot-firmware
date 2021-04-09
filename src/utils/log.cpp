#include "./log.h"

#ifdef IOT_NODE_LOGGING

namespace IotNode {
namespace Utils {

namespace Log {
  void setup() {
    Serial.begin(IOT_NODE_SERIAL0_BAUD_RATE);

    #if defined(IOT_NODE_ESP8266) && defined(IOT_NODE_BOARD_H801)
      Serial.set_tx(2);
    #endif

    #ifdef IOT_NODE_ESP8266
      delay(IOT_NODE_LOG_DELAY);
    #endif
    #ifdef IOT_NODE_ESP32
      vTaskDelay(IOT_NODE_LOG_DELAY / portTICK_PERIOD_MS);
    #endif
    
    info();
  }

  void debug(String key, String value) {
    unsigned long now = millis();

    Serial.print("[");
    Serial.print(now);
    Serial.print("]:\"");
    Serial.print(key);
    Serial.print("\":\"");
    Serial.print(value);
    Serial.print("\"\n");
  }

  void info() {
    yield();

    Serial.println();

    debug("info.name", IOT_NODE_NAME);

    #ifdef IOT_NODE_BOARD_NAME
      debug("info.board-name", IOT_NODE_BOARD_NAME);
    #endif

    #ifdef IOT_NODE_HARDWARE_NAME
      debug("info.hardware-name", IOT_NODE_HARDWARE_NAME);
    #endif

    debug("info.build.git-rev", IOT_NODE_BUILD_GIT_REV);
    debug("info.build.pio.env", IOT_NODE_PIO_ENV);
    debug("info.build.pio.platform", IOT_NODE_PIO_PLATFORM);
    debug("info.build.pio.framework", IOT_NODE_PIO_FRAMEWORK);

    #ifdef IOT_NODE_ESP8266
      debug("info.system.chip-id", String(ESP.getChipId(), HEX));
      debug("info.system.flash-id", String(ESP.getFlashChipId(), HEX));
    #endif

    debug("info.system.mac-address", WiFi.macAddress());
  }
}

} // section namespace
} // project namespace

#endif
