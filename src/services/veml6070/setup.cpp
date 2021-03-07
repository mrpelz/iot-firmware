#ifdef IOT_NODE_VEML6070

#include "./setup.h"

namespace IotNode {

namespace Veml6070 {
  UDP::Service service = {
    .serviceId = serviceIds::veml6070,
    .handler = handler,
  };

  void setup(UDP::Class *udp, TwoWire *i2c) {
    initializer(i2c);

    udp->addService(&service);
  }
}

} // project namespace

#endif
