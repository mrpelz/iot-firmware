#ifdef IOT_NODE_SGP30

#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Sgp30 {
  Utils::UDP::Service service = {
    .serviceId = ids::sgp30,
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
