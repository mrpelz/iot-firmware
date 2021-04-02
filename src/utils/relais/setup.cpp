#include "./setup.h"

#ifdef IOT_NODE_RELAIS

namespace IotNode {
namespace Utils {

namespace Relais {
  #ifdef IOT_NODE_BOARD_SHELLY1
    Class relais0({ 4, false });
  #endif

  void setup() {
    #ifdef IOT_NODE_BOARD_SHELLY1
      relais0.init();
    #endif
  }
}

} // section namespace
} // project namespace

#endif
