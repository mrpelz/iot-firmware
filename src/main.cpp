#include "./main.h"

#ifndef IOT_NODE_DHCP
  InterfaceConfig interfaceConfig = {
    IPAddress(10, 97, 4, 254),
    IPAddress(10, 97, 4, 1),
    IPAddress(255, 255, 255, 0),
  };
#endif

Timings timings = {
  60000, // runDebugEvery
  2000, // tryReconnectAfter
  10000 // restartAfter
};

#ifdef IOT_NODE_LINK_ETH
  #ifdef ARDUINO_ARCH_ESP32
    PersistentEth persistentLink({
      #ifndef IOT_NODE_DHCP
        interfaceConfig,
      #endif
      timings
    });
  #endif
#else
  PersistentWiFi persistentLink({
    #ifdef ARDUINO_ARCH_ESP8266
      WIFI_PHY_MODE_11N, // phyMode
      6, // outputPower (dBm)
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      WIFI_PHY_RATE_54M, // phyMode
      WIFI_POWER_7dBm,
    #endif
    #ifndef IOT_NODE_DHCP
      interfaceConfig,
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
    timings
  });
#endif

UDPMessaging udp(8266);
UDPService relais0Service;

ButtonTiming buttons({
  50, // debounceTime
  3000, // repeatTime
  125, // longpressTime (step duration)
  {
    {
      0,
      5,
      true,
      false
    }
  }
});

void possiblyDeferredSetup() {
  setupInfoLog();

  persistentLink.setDebug(debug);
  udp.setDebug(debug);
  buttons.setDebug(debug);

  #ifdef IOT_NODE_DEFER_INITIAL_LOGGING
    persistentLink.debug(true);
  #endif

  yield();

  relais0Service = makeRelaisService(0, 4, false);
  buttons.start();
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

  persistentLink.onGotIP([]() {
    #ifdef IOT_NODE_DEFER_INITIAL_LOGGING
      if (infoLog != 0) return;
      infoLog = 1;
    #endif

    udp.begin();
  });

  persistentLink.onDisconnected([]() {
    udp.close();
  });

  udp.addService(&helloService);
  udp.addService(&systemInfoService);
  udp.addService(&asyncService);
  udp.addService(&relais0Service);
  udp.addService(&keepAliveService);

  buttons.setChangeCallback([](
    uint8_t index,
    bool down,
    bool downChanged,
    unsigned long prevDuration,
    uint8_t repeat,
    uint8_t longpress
  ) {
    buttonEvent(
      &udp,
      index,
      down,
      downChanged,
      repeat,
      longpress,
      prevDuration
    );
  });

  persistentLink.connect();
}

void loop() {
  persistentLink.update();
  yield();

  #ifdef IOT_NODE_DEFER_INITIAL_LOGGING
    possiblyDeferredSetup();
    yield();
  #endif

  TimeoutUpdate();
  yield();

  asyncUpdate();
  yield();

  buttons.update();
  yield();
}
