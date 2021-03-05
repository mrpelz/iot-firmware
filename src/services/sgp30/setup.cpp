#ifdef IOT_NODE_SGP30

#include "./setup.h"

namespace IotNode {

namespace Sgp30 {
  UDP::Service service = {
    .serviceId = serviceIds::sgp30,
    .handler = handler,
  };

  void setup(UDP::Class *udp, TwoWire *i2c) {
    initializer(i2c);

    udp->addService(&service);
  }
}

} // project namespace

#endif
