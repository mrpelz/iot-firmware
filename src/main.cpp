#if defined(IOT_NODE_AP_ALGORE) || defined(IOT_NODE_AP_ELZAR) || defined(IOT_NODE_AP_RICHARDNIXON) || defined(IOT_NODE_AP_SPIROAGNEW)
  #define IOT_NODE_ADVANCED_WIFI_CONFIG
#endif

 #define TOSTRING(x) #x
 #define STR(x) TOSTRING(x)

#include <Arduino.h>
#include "./persistent-link.cpp"
#include "./udp-messaging.cpp"

PersistentLink persistentWifi({
  WIFI_PHY_MODE_11N, // phyMode
  6, // outputPower (dBm)
  #ifndef IOT_NODE_DHCP
    {
      IPAddress(10, 97, 4, 254),
      IPAddress(10, 97, 4, 1),
      IPAddress(255, 255, 255, 0),
    },
  #endif
  {
    "iot.wurstsalat.cloud", // ssid iot
    "xyz", // password iot
    #if defined(IOT_NODE_AP_ALGORE)
      { 0x78, 0x8a, 0x20, 0x83, 0x69, 0x8c }, // bssid algore
      11, // channel algore
    #elif defined(IOT_NODE_AP_ELZAR)
      { 0x78, 0x8a, 0x20, 0x2c, 0x4b, 0x91 }, // bssid elzar
      1, // channel elzar
    #elif defined(IOT_NODE_AP_RICHARDNIXON)
      { 0x78, 0x8a, 0x20, 0x81, 0xd3, 0x2b }, // bssid richardnixon
      6, // channel richardnixon
    #elif defined(IOT_NODE_AP_SPIROAGNEW)
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

UDPMessaging udp({
  8266
});

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

  persistentWifi.setDebug(debug);
  udp.setDebug(debug);

  #ifdef IOT_NODE_DEFER_INITIAL_LOGGING
    persistentWifi.wifiDebug(true);
  #endif
}

void setup() {
  Serial.begin(74880);

  #ifndef IOT_NODE_DEFER_INITIAL_LOGGING
    setupInfoLog();
  #endif

  persistentWifi.onGotIP([](WiFiEventStationModeGotIP event) {
    #ifdef IOT_NODE_DEFER_INITIAL_LOGGING
      if (infoLog != 0) return;
      infoLog = 1;
    #endif

    udp.begin();
  });

  persistentWifi.connect();

  udp.addService({
    1,
    [](
      UDPIncomingMessage request,
      UDPOutgoingMessage response
    ) {
      debug("udp-service.got", String(request.payload.message[0]));

      response.payloadLength = 1;
      response.payload.message[0] = 44;

      udp.write(response);

      // yield();

      // UDPEventMessage event;

      // event.remoteAddress = udp.lastPeerAddress();
      // event.remotePort = udp.lastPeerPort();
      // event.payload.eventId = 90;
      // event.payload.message[0] = 5;
      // event.payloadLength = 1;

      // udp.event(event);
    }
  });
}

void loop() {
  persistentWifi.update();
  yield();

  udp.update();
  yield();

  #ifdef IOT_NODE_DEFER_INITIAL_LOGGING
    setupInfoLog();
    yield();
  #endif
}
