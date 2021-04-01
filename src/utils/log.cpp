#include "./log.h"

namespace IotNode {
namespace Utils {

namespace Log {
  void setup() {
    #ifdef IOT_NODE_ESP8266
      Serial.begin(74880);
    #endif
    #ifdef IOT_NODE_ESP32
      Serial.begin(115200);
    #endif

    #ifdef IOT_NODE_ESP8266
      delay(LOG_DELAY);
    #endif
    #ifdef IOT_NODE_ESP32
      vTaskDelay(LOG_DELAY / portTICK_PERIOD_MS);
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
    debug("info.build.git-rev", STR(IOT_NODE_BUILD_GIT_REV));
    debug("info.build.pio.env", STR(IOT_NODE_PIO_ENV));
    debug("info.build.pio.platform", STR(IOT_NODE_PIO_PLATFORM));
    debug("info.build.pio.framework", STR(IOT_NODE_PIO_FRAMEWORK));

    #ifdef IOT_NODE_ESP8266
      debug("info.system.chip-id", String(ESP.getChipId(), HEX));
      debug("info.system.flash-id", String(ESP.getFlashChipId(), HEX));
    #endif

    debug("info.system.mac-address", WiFi.macAddress());
  }
}

} // section namespace
} // project namespace
