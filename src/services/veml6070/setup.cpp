#ifdef IOT_NODE_VEML6070

#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Veml6070 {
  Utils::UDP::Service service = {
    .serviceId = ids::veml6070,
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
