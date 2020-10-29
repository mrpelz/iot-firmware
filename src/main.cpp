// #define USE_AP_ALGORE
// #define USE_AP_ELZAR
#define USE_AP_RICHARDNIXON
// #define USE_AP_SPIROAGNEW

// #define USE_DHCP

#define DEFER_INITIAL_LOGGING

#if defined(USE_AP_ALGORE) || defined(USE_AP_ELZAR) || defined(USE_AP_RICHARDNIXON) || defined(USE_AP_SPIROAGNEW)
  #define USE_ADVANCED_WIFI_CONFIG
#endif

#include <Arduino.h>
#include "./persistent-wifi.cpp"

PersistentWiFi persistentWifi({
  WIFI_PHY_MODE_11N, // phyMode
  6, // outputPower (dBm)
  #ifndef USE_DHCP
    {
      IPAddress(10, 97, 0, 254),
      IPAddress(10, 97, 0, 1),
      IPAddress(255, 255, 255, 0),
    },
  #endif
  {
    "iot.wurstsalat.cloud", // ssid iot
    "xyz", // password iot
    #if defined(USE_AP_ALGORE)
      { 0x78, 0x8a, 0x20, 0x83, 0x69, 0x8c }, // bssid algore
      11, // channel algore
    #elif defined(USE_AP_ELZAR)
      { 0x78, 0x8a, 0x20, 0x2c, 0x4b, 0x91 }, // bssid elzar
      1, // channel elzar
    #elif defined(USE_AP_RICHARDNIXON)
      { 0x78, 0x8a, 0x20, 0x81, 0xd3, 0x2b }, // bssid richardnixon
      6, // channel richardnixon
    #elif defined(USE_AP_SPIROAGNEW)
      { 0xf0, 0x9f, 0xc2, 0xc8, 0xb6, 0x18 }, // bssid spiroagnew
      6, // channel spiroagnew
    #endif
  },
  {
    60000, // runWifiDebugEvery
    2000, // tryWifiReconnectAfter
    10000 // restartAfter
  }
});

#ifdef DEFER_INITIAL_LOGGING
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
  #ifdef DEFER_INITIAL_LOGGING
    if (infoLog != 1) return;
    infoLog = 2;
  #endif

  yield();

  Serial.println();

  #ifdef DEFER_INITIAL_LOGGING
    debug("info.ititial-loging", "deferred");
  #else
    debug("info.ititial-loging", "real-time");
  #endif

  debug("info.system.chip-id", String(ESP.getChipId(), HEX));
  debug("info.system.flash-id", String(ESP.getFlashChipId(), HEX));
  debug("info.system.mac-address", WiFi.macAddress());

  persistentWifi.setDebug(debug);

  #ifdef DEFER_INITIAL_LOGGING
    persistentWifi.wifiDebug(true);
  #endif
}

void setup() {
  Serial.begin(74880);

  #ifndef DEFER_INITIAL_LOGGING
    setupInfoLog();
  #endif

  #ifdef DEFER_INITIAL_LOGGING
    persistentWifi.onGotIP([](WiFiEventStationModeGotIP event) {
      if (infoLog != 0) return;
      infoLog = 1;
    });
  #endif

  persistentWifi.connect();
}

void loop() {
  persistentWifi.update();

  #ifdef DEFER_INITIAL_LOGGING
    setupInfoLog();
  #endif

  yield();
}
