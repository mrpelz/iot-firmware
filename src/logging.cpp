#include "./logging.h"

#ifdef IOT_NODE_DEFER_INITIAL_LOGGING
  // 0: wifi not started yet, defer infoLog
  // 1: wifi started, run infoLog
  // 2: infoLog has already been executed, do nothing
  uint8_t infoLog = 0;
#endif

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
  #ifdef IOT_NODE_DEFER_INITIAL_LOGGING
    if (infoLog != 1) return;
    infoLog = 2;
  #endif

  yield();

  Serial.println();

  #ifdef IOT_NODE_DEFER_INITIAL_LOGGING
    debug("info.ititial-loging", "deferred");
  #else
    debug("info.ititial-loging", "real-time");
  #endif

  #ifdef IOT_NODE_BUILD_TIME
    debug("info.build.time", STR(IOT_NODE_BUILD_TIME));
  #endif

  debug("info.build.git-rev", STR(IOT_NODE_BUILD_GIT_REV));
  debug("info.build.pio.env", STR(IOT_NODE_PIO_ENV));
  debug("info.build.pio.platform", STR(IOT_NODE_PIO_PLATFORM));
  debug("info.build.pio.framework", STR(IOT_NODE_PIO_FRAMEWORK));
  debug("info.system.chip-id", String(ESP.getChipId(), HEX));
  debug("info.system.flash-id", String(ESP.getFlashChipId(), HEX));
  debug("info.system.mac-address", WiFi.macAddress());
}
