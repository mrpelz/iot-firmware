#ifdef IOT_NODE_SDS011

#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Sds011 {
  Utils::UDP::Service service = {
    .serviceId = ids::sds011,
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
