#include "./setup.h"

#ifdef IOT_NODE_OUTPUT

namespace IotNode {
namespace Utils {

namespace Output {
  #ifdef IOT_NODE_BOARD_SHELLY1
    Regular output0({ 4, false });
  #elif defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V13)
    Regular output0({ 12, false });
  #elif defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V1)
    Regular output0({ 12, false });
  #elif defined(IOT_NODE_BOARD_SHELLY_PLUS_1)
    Regular output0({ 26, false });
  #elif defined(IOT_NODE_BOARD_OBI_JACK)
    Pulse output0({ 12, 5, true });
  #endif

  void setup() {
    output0.init();
  }
}

} // section namespace
} // project namespace

#endif
