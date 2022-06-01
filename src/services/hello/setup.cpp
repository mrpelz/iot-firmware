#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Hello {
  Utils::UDP::Service service = {
    .serviceId = ids::hello,
    .serviceIndex = 0,
    .handler = handler,
  };

  void setup() {
    Utils::UDP::instance.addService(&service);
  }
}

} // section namespace
} // project namespace
