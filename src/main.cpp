#include "./main.h"

#ifdef IOT_NODE_LINK_ETH
  #ifdef ARDUINO_ARCH_ESP32
    PersistentEth persistentLink(persistentLinkConfig);
  #endif
#else
  PersistentWiFi persistentLink(persistentLinkConfig);
#endif

UDPMessaging udp(8266);

Relais relais0({ 4, false });
auto relais0Service = makeRelaisService(&relais0, 0);

Buttons buttons(buttons);

void possiblyDeferredSetup() {
  setupInfoLog();

  persistentLink.setDebug(debug);
  udp.setDebug(debug);
  buttons.setDebug(debug);

  #ifdef IOT_NODE_DEFER_INITIAL_LOGGING
    persistentLink.debug(true);
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
    delay(100);
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

  buttons.setChangeCallback([](ButtonUpdate update) {
    if (udp.isListening() && udp.hasEventPeer()) {
      buttonEvent(&udp, update);
      return;
    }

    if (
      update.index == 0
      && update.downChanged
      && !update.down
    ) {
      relais0.toggle();
    }
  });

  persistentLink.connect();
  relais0.init();
  buttons.start();
}

void loop() {
  persistentLink.update();
  yield();

  #ifdef IOT_NODE_DEFER_INITIAL_LOGGING
    possiblyDeferredSetup();
    yield();
  #endif

  timeoutUpdate();
  yield();

  asyncUpdate();
  yield();

  buttons.update();
  yield();
}
