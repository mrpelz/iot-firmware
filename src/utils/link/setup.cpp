#include "./setup.h"

namespace IotNode {

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

  void setup(UDP::Class *udp) {
    link.setDebug(Log::debug);

    link.onGotIP([udp]() {
      udp->begin();
    });

    link.onDisconnected([udp]() {
      udp->close();
    });

    link.connect();

    xTaskCreate(task, "link_maintenance", 10000, NULL, 2, NULL);
  }
}

} // project namespace
