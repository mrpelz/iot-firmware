#include "./setup.h"

#ifdef IOT_NODE_INDICATORS

namespace IotNode {
namespace Services {

namespace Indicator {
  auto service0 = makeService(&Utils::Indicator::indicator0, 0);

  void setup() {
    Utils::UDP::instance.addService(&service0);
  }
}

} // section namespace
} // project namespace

#endif
