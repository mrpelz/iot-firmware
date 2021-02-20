#include "./setup.h"

namespace Link {
  #ifdef IOT_NODE_LINK_ETH
    #ifdef ARDUINO_ARCH_ESP32
      Class link(config);
    #endif
  #else
    Class link(config);
  #endif

  void task(void * parameter) {
    for(;;) {
      link.update();
      yield();
    }
  }

  void setup(UDPMessaging *udp) {
    link.setDebug(debug);

    link.onGotIP([udp]() {
      udp->begin();
    });

    link.onDisconnected([udp]() {
      udp->close();
    });

    link.connect();

    xTaskCreate(task, "link_maintenance", 10000, NULL, 1, NULL);
  }
}
