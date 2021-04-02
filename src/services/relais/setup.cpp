#include "./setup.h"

#ifdef IOT_NODE_RELAIS

namespace IotNode {
namespace Services {

namespace Relais {
  #ifdef IOT_NODE_BOARD_SHELLY1
    auto service0 = makeService(&Utils::Relais::relais0, 0);
  #endif

  void setup() {
    #ifdef IOT_NODE_BOARD_SHELLY1
      Utils::UDP::instance.addService(&service0);
    #endif
  }
}

} // section namespace
} // project namespace

#endif
