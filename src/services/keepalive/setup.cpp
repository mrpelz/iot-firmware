#include "./setup.h"

namespace IotNode {

namespace Keepalive {
  Class restartOnTimeout(120000);
  const TickType_t delay = 10 / portTICK_PERIOD_MS;

  UDP::Service service = {
    .serviceId = serviceIds::keepalive,
    .handler = makeHandler(&restartOnTimeout),
  };

  void task(void * parameter) {
    for(;;) {
      restartOnTimeout.update();
      vTaskDelay(delay);
    }
  }

  void setup(UDP::Class *udp) {
    udp->addService(&service);

    xTaskCreate(task, "keepalive_maintenance", 2048, NULL, 1, NULL);
  }
}

} // project namespace
