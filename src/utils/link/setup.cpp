#include "./setup.h"

namespace IotNode {
namespace Utils {

namespace Link {
  #ifdef IOT_NODE_LINK_ETH
    #ifdef ARDUINO_ARCH_ESP32
      Class link(config);
    #endif
  #else
    Class link(config);
  #endif

  void update() {
    link.update();
  }

  #ifdef ARDUINO_ARCH_ESP32
    void task(void *parameter) {
      for(;;) {
        update();
        vTaskDelay(10 / portTICK_PERIOD_MS);
      }
    }
  #endif

  void setup(UDP::Class *udp) {
    link.setDebug(Log::debug);

    link.onGotIP([udp]() {
      udp->begin();
      Indicator::rxdLed.setOn(false);
      Indicator::txdLed.blink(3);
    });

    link.onDisconnected([udp]() {
      udp->close();
      Indicator::rxdLed.blink();
    });

    link.connect();

    #ifdef ARDUINO_ARCH_ESP32
      xTaskCreatePinnedToCore(
        task,
        "link_maintenance",
        2048,
        NULL,
        2,
        NULL,
        CONFIG_ARDUINO_RUNNING_CORE
      );
    #endif
  }
}

} // section namespace
} // project namespace
