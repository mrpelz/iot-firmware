#ifdef IOT_NODE_BOARD_SHELLY1

#include "./output.h"

namespace IotNode::Setup::Output
{
  ::IotNode::Utils::Output::Regular output0({4, false});
  auto service0 = ::IotNode::Services::Output::makeService(&output0, 0);

  void setup()
  {
    output0.init();
    ::IotNode::Utils::UDP::instance.addService(&service0);
  }
}

#endif
