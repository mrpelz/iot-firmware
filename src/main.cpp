#include "./main.h"

UDPMessaging udp(8266);

Relais relais0({ 4, false });
auto relais0Service = makeRelaisService(&relais0, 0);

Buttons buttons(buttonsConfig);

void setup() {
  #ifdef ARDUINO_ARCH_ESP8266
    Serial.begin(74880);
  #endif
  #ifdef ARDUINO_ARCH_ESP32
    Serial.begin(115200);
  #endif

  delay(100);
  
  setupInfoLog();

  Link::setup(&udp);

  udp.setDebug(debug);
  buttons.setDebug(debug);

  udp.addService(&helloService);
  udp.addService(&systemInfoService);
  udp.addService(&asyncService);
  udp.addService(&mcp9808Service);
  udp.addService(&bme280Service);
  udp.addService(&tsl2561Service);
  udp.addService(&relais0Service);
  udp.addService(&keepAliveService);

  buttons.setChangeCallback([](ButtonUpdate update) {
    if (udp.isListening() && udp.hasEventPeer()) {
      buttonEvent(&udp, update);
      return;
    }

    debug("info.buttons.change-callback", "udp event not usable");

    if (
      update.index == 0
      && update.downChanged
      && !update.down
    ) {
      debug("info.buttons.change-callback", "triggering override");
      relais0.toggle();
    }
  });

  relais0.init();
  buttons.start();
}

void loop() {
  timeoutUpdate();
  yield();

  asyncUpdate();
  yield();

  buttons.update();
  yield();

  tsl2561UpdateHandler.update();
  yield();
}
