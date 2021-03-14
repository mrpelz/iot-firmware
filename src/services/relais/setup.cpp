#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Relais {
  auto service0 = makeService(&Utils::Relais::relais0, 0);
  auto service1 = makeService(&Utils::Relais::relais1, 1);

  void setup() {
    Utils::UDP::instance.addService(&service0);
    Utils::UDP::instance.addService(&service1);
  }
}

} // section namespace
} // project namespace
