#ifdef IOT_NODE_CCS811

#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Ccs811 {
  Utils::UDP::Service service = {
    .serviceId = ids::ccs811,
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
