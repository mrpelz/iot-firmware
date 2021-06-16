#include "./setup.h"

#ifdef IOT_NODE_OUTPUT

namespace IotNode {
namespace Services {

namespace Output {
  auto service0 = makeService(&Utils::Output::output0, 0);

  void setup() {
    Utils::UDP::instance.addService(&service0);
  }
}

} // section namespace
} // project namespace

#endif
