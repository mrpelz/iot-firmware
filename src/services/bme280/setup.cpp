#ifdef IOT_NODE_BME280

#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Bme280 {
  Utils::UDP::Service service = {
    .serviceId = ids::bme280,
    .handler = handler,
  };

  void setup(Utils::UDP::Class *udp, TwoWire *i2c) {
    initializer(i2c);

    udp->addService(&service);
  }
}

} // section namespace
} // project namespace

#endif
