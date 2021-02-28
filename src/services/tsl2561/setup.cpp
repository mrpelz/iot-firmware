#ifdef IOT_NODE_TSL2561

#include "./setup.h"

namespace IotNode {

namespace Tsl2561 {
  UDP::Service service = {
    .serviceId = serviceIds::tsl2561,
    .handler = handler,
  };

  void setup(UDP::Class *udp) {
    initializer();

    udp->addService(&service);
  }
}

} // project namespace

#endif
