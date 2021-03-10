#ifdef IOT_NODE_VEML6070

#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Veml6070 {
  Utils::UDP::Service service = {
    .serviceId = serviceIds::veml6070,
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
