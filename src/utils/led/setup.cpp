#include "./setup.h"

#ifdef IOT_NODE_LED

namespace IotNode {
namespace Utils {

namespace Led {
  #ifdef IOT_NODE_BOARD_H801
    Class led0({ 12, false });
    Class led1({ 15, false });
    Class led2({ 13, false });
    Class led3({ 14, false });
    Class led4({ 4, false });
  #endif

  void setup() {
    #ifdef IOT_NODE_BOARD_H801
      led0.init();
      led1.init();
      led2.init();
      led3.init();
      led4.init();
    #endif
  }
}

} // section namespace
} // project namespace

#endif
