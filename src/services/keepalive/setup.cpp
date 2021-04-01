#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Keepalive {
  Class restartOnTimeout(120000);

  Utils::UDP::Service service = {
    .serviceId = ids::keepalive,
    .handler = makeHandler(&restartOnTimeout),
  };

  void update() {
    restartOnTimeout.update();
  }

  #ifdef ARDUINO_ARCH_ESP32
    void task(void *parameter) {
      for(;;) {
        update();
        vTaskDelay(IOT_NODE_MUTLITASKING_DELAY / portTICK_PERIOD_MS);
      }
    }
  #endif

  void setup() {
    Utils::UDP::instance.addService(&service);

    #ifdef ARDUINO_ARCH_ESP32
      xTaskCreatePinnedToCore(
        task,
        "keepalive_maintenance",
        2048,
        NULL,
        1,
        NULL,
        CONFIG_ARDUINO_RUNNING_CORE
      );
    #endif
  }
}

} // section namespace
} // project namespace
