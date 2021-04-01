#include "./setup.h"

namespace IotNode {
namespace Utils {

namespace Link {
  Class link(config);

  void update() {
    link.update();
  }

  #ifdef IOT_NODE_ESP32
    void task(void *parameter) {
      for(;;) {
        update();
        vTaskDelay(IOT_NODE_MUTLITASKING_DELAY / portTICK_PERIOD_MS);
      }
    }
  #endif

  void setup() {
    link.setDebug(Log::debug);

    link.onGotIP([]() {
      UDP::instance.begin();
      Indicator::rxdLed.setOn(false);
      Indicator::txdLed.blink(3);
    });

    link.onDisconnected([]() {
      UDP::instance.close();
      Indicator::rxdLed.blink();
    });

    link.connect();

    #ifdef IOT_NODE_ESP32
      xTaskCreatePinnedToCore(
        task,
        "link_maintenance",
        4096,
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
