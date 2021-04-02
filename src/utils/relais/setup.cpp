#include "./setup.h"

#ifdef IOT_NODE_RELAIS

namespace IotNode {
namespace Utils {

namespace Relais {
  #ifdef IOT_NODE_BOARD_SHELLY1
    Regular relais0({ 4, false });
  #elif defined(IOT_NODE_BOARD_OBI_JACK)
    Pulse relais0({ 12, 5, true });
  #endif

  void setup() {
    relais0.init();
  }
}

} // section namespace
} // project namespace

#endif
