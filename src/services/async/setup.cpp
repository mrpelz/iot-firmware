#ifdef ARDUINO_ARCH_ESP32

#include "./setup.h"

namespace IotNode {

namespace Async {
  UDP::Service service = {
    .serviceId = serviceIds::async,
    .handler = handler,
  };

  void setup(UDP::Class *udp) {
    udp->addService(&service);
  }
}

} // project namespace

#endif
