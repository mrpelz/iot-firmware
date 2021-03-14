#ifdef IOT_NODE_TSL2561

#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Tsl2561 {
  Utils::UDP::Service service = {
    .serviceId = ids::tsl2561,
    .handler = handler,
  };

  void setup() {
    initializer(&Utils::I2C::bus);

    Utils::UDP::instance.addService(&service);
  }
}

} // section namespace
} // project namespace

#endif
