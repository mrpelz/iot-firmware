#include "./logging.h"

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

void setupInfoLog() {
  yield();

  Serial.println();

  #ifdef IOT_NODE_BUILD_TIME
    debug("info.build.time", STR(IOT_NODE_BUILD_TIME));
  #endif

  debug("info.build.git-rev", STR(IOT_NODE_BUILD_GIT_REV));
  debug("info.build.pio.env", STR(IOT_NODE_PIO_ENV));
  debug("info.build.pio.platform", STR(IOT_NODE_PIO_PLATFORM));
  debug("info.build.pio.framework", STR(IOT_NODE_PIO_FRAMEWORK));

  #ifdef ARDUINO_ARCH_ESP8266
    debug("info.system.chip-id", String(ESP.getChipId(), HEX));
    debug("info.system.flash-id", String(ESP.getFlashChipId(), HEX));
  #endif

  debug("info.system.mac-address", WiFi.macAddress());
}
