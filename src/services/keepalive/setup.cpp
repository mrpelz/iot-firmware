#include "./setup.h"

namespace IotNode {

namespace Keepalive {
  Class restartOnTimeout(120000);

  UDP::Service service = {
    .serviceId = serviceIds::keepalive,
    .handler = makeHandler(&restartOnTimeout),
  };

  void task(void * parameter) {
    for(;;) {
      restartOnTimeout.update();
      yield();
    }
  }

  void setup(UDP::Class *udp) {
    udp->addService(&service);

    xTaskCreate(task, "keepalive_maintenance", 10000, NULL, 1, NULL);
  }
}

} // project namespace
