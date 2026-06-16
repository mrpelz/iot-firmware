#ifdef IOT_NODE_BOARD_OBI_JACK

#include "./output.h"

namespace IotNode::Setup::Output
{
  ::IotNode::Utils::Output::Pulse output0({12, 5, true});

  auto service0 = ::IotNode::Services::Output::makeService(&output0, 0);

  void setup()
  {
    output0.init();
    ::IotNode::Utils::UDP::instance.addService(&service0);
  }
}

#endif
