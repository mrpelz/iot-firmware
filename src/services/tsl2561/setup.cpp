#ifdef IOT_NODE_TSL2561

#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Tsl2561 {
  Utils::UDP::Service service = {
    .serviceId = serviceIds::tsl2561,
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
