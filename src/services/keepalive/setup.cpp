#include "./setup.h"

namespace IotNode {

namespace Keepalive {
  Class restartOnTimeout(120000);

  UDP::Service service = {
    .serviceId = serviceIds::keepalive,
    .handler = makeHandler(&restartOnTimeout),
  };

  void update() {
    restartOnTimeout.update();
  }

  #ifdef ARDUINO_ARCH_ESP32
    void task(void * parameter) {
      for(;;) {
        update();
        vTaskDelay(10 / portTICK_PERIOD_MS);
      }
    }
  #endif

  void setup(UDP::Class *udp) {
    udp->addService(&service);

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

} // project namespace
