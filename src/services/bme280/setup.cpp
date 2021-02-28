#include "./setup.h"

namespace IotNode {

namespace Bme280 {
  UDP::Service service = {
    .serviceId = serviceIds::bme280,
    .handler = handler,
  };

  void setup(UDP::Class *udp) {
    initializer();

    udp->addService(&service);
  }
}

} // project namespace
