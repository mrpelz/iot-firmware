#include "./setup.h"

namespace IotNode {

namespace Blink {
  UDP::Service service = {
    .serviceId = serviceIds::blink,
    .handler = handler,
  };

  void setup(UDP::Class *udp) {
    udp->addService(&service);
  }
}

} // project namespace
