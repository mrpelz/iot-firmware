#ifdef IOT_NODE_MHZ19

#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Mhz19 {
  Utils::UDP::Service service = {
    .serviceId = ids::mhz19,
    .handler = handler,
  };

  void setup() {
    initializer();

    Utils::UDP::instance.addService(&service);
  }
}

} // section namespace
} // project namespace

#endif
