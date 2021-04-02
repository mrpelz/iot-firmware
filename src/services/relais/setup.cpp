#include "./setup.h"

#ifdef IOT_NODE_RELAIS

namespace IotNode {
namespace Services {

namespace Relais {
  auto service0 = makeService(&Utils::Relais::relais0, 0);

  void setup() {
    Utils::UDP::instance.addService(&service0);
  }
}

} // section namespace
} // project namespace

#endif
