#ifndef _MAIN
#define _MAIN

#include "./main.h"

PersistentLink persistentWifi({
  #ifdef ARDUINO_ARCH_ESP8266
    WIFI_PHY_MODE_11N, // phyMode
    6, // outputPower (dBm)
  #endif
  #ifdef ARDUINO_ARCH_ESP32
    WIFI_PHY_RATE_54M, // phyMode
    WIFI_POWER_7dBm,
  #endif
  #ifndef IOT_NODE_DHCP
    {
      IPAddress(10, 97, 4, 254),
      IPAddress(10, 97, 4, 1),
      IPAddress(255, 255, 255, 0),
    },
  #endif
  {
    STR(IOT_NODE_WIFI_SSID),
    STR(IOT_NODE_WIFI_PSK),
    #if defined(IOT_NODE_AP_ALGORE)
      // { 0x78, 0x8a, 0x20, 0x83, 0x69, 0x8c }, // bssid algore iot
      { 0x7a, 0x8a, 0x20, 0x84, 0x69, 0x8c }, // bssid algore legacy
      11, // channel algore
    #elif defined(IOT_NODE_AP_ELZAR)
      // { 0x78, 0x8a, 0x20, 0x2c, 0x4b, 0x91 }, // bssid elzar iot
      { 0x7a, 0x8a, 0x20, 0x2d, 0x4b, 0x91 }, // bssid elzar legacy
      1, // channel elzar
    #elif defined(IOT_NODE_AP_RICHARDNIXON)
      // { 0x78, 0x8a, 0x20, 0x81, 0xd3, 0x2b }, // bssid richardnixon iot
      { 0x7a, 0x8a, 0x20, 0x81, 0xd3, 0x2b }, // bssid richardnixon legacy
      6, // channel richardnixon
    #elif defined(IOT_NODE_AP_SPIROAGNEW)
      // { 0xf0, 0x9f, 0xc2, 0xc8, 0xb6, 0x18 }, // bssid spiroagnew iot
      { 0xf2, 0x9f, 0xc2, 0xc9, 0xb6, 0x18 }, // bssid spiroagnew legacy
      6, // channel spiroagnew
    #endif
  },
  {
    60000, // runWifiDebugEvery
    2000, // tryWifiReconnectAfter
    10000 // restartAfter
  }
});

UDPMessaging udp(8266);

void possiblyDeferredSetup() {
  setupInfoLog();

  persistentWifi.setDebug(debug);
  udp.setDebug(debug);

  #ifdef IOT_NODE_DEFER_INITIAL_LOGGING
    persistentWifi.wifiDebug(true);
  #endif
}

void setup() {
  #ifdef ARDUINO_ARCH_ESP8266
    Serial.begin(74880);
  #endif
  #ifdef ARDUINO_ARCH_ESP32
    Serial.begin(115200);
  #endif

  #ifndef IOT_NODE_DEFER_INITIAL_LOGGING
    possiblyDeferredSetup();
  #endif

  persistentWifi.onGotIP([]() {
    #ifdef IOT_NODE_DEFER_INITIAL_LOGGING
      if (infoLog != 0) return;
      infoLog = 1;
    #endif

    udp.begin();
  });

  persistentWifi.onDisconnected([]() {
    udp.close();
  });

  udp.addService(&helloService);
  udp.addService(&systemInfoService);
  udp.addService(&asyncService);
  udp.addService(&keepAliveService);

  persistentWifi.connect();
}

void loop() {
  TimeoutUpdate();
  yield();

  persistentWifi.update();
  yield();

  #ifdef IOT_NODE_DEFER_INITIAL_LOGGING
    possiblyDeferredSetup();
    yield();
  #endif

  asyncUpdate();
  yield();
}

#endif
