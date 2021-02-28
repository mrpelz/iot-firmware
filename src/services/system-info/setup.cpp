#include "./setup.h"

namespace IotNode {

namespace SystemInfo {
  UDP::Service service = {
    .serviceId = serviceIds::systemInfo,
    .handler = handler,
  };

  void setup(UDP::Class *udp) {
    udp->addService(&service);
  }
}

} // project namespace
