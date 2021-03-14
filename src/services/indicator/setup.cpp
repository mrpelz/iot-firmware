#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Indicator {
  auto service0 = makeService(&Utils::Indicator::rxdLed, 0);
  auto service1 = makeService(&Utils::Indicator::txdLed, 1);

  void setup() {
    Utils::UDP::instance.addService(&service0);
    Utils::UDP::instance.addService(&service1);
  }
}

} // section namespace
} // project namespace
