#ifdef IOT_NODE_CCS811

#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Ccs811 {
  Utils::UDP::Service service = {
    .serviceId = ids::ccs811,
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
