#include "./setup.h"

#ifdef IOT_NODE_INDICATORS

namespace IotNode {
namespace Services {

namespace Indicator {
  #if defined(IOT_NODE_BOARD_ROOM_SENSOR) || defined(IOT_NODE_BOARD_TEST_DEVICE)
    auto service0 = makeService(&Utils::Indicator::indicator0, 0);
  #endif

  void setup() {
    #if defined(IOT_NODE_BOARD_ROOM_SENSOR) || defined(IOT_NODE_BOARD_TEST_DEVICE)
      Utils::UDP::instance.addService(&service0);
    #endif
  }
}

} // section namespace
} // project namespace

#endif
