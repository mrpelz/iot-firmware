#if defined(IOT_NODE_BOARD_SHELLY1) && defined(IOT_NODE_INPUT)

#include "./input.h"

namespace IotNode::Setup::Input
{
  ::IotNode::Events::Input::Class input0(3, true, 0, 300);

  void update()
  {
    input0.update();
  }

  void setup()
  {
    auto event0 = ::IotNode::Events::Input::makeEvent(&::IotNode::Utils::UDP::instance, 0);
    input0.setChangeCallback(event0);
    input0.start();
  }
}

#endif
