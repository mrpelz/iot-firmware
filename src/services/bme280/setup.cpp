#ifdef IOT_NODE_BME280

#include "./setup.h"

namespace IotNode {

namespace Bme280 {
  UDP::Service service = {
    .serviceId = serviceIds::bme280,
    .handler = handler,
  };

  void setup(UDP::Class *udp, TwoWire *i2c) {
    initializer(i2c);

    udp->addService(&service);
  }
}

} // project namespace

#endif
