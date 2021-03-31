#ifdef ARDUINO_ARCH_ESP32

#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Async {
  Utils::UDP::Service service = {
    .serviceId = ids::async,
    .handler = handler,
  };

  void setup() {
    Utils::UDP::instance.addService(&service);

    initializer();
  }
}

} // section namespace
} // project namespace

#endif
