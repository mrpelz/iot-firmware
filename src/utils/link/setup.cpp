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

  const TickType_t delay = 10 / portTICK_PERIOD_MS;

  void task(void * parameter) {
    for(;;) {
      link.update();
      vTaskDelay(delay);
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

    xTaskCreate(task, "link_maintenance", 2048, NULL, 2, NULL);
  }
}

} // project namespace
