#include "./setup.h"

#ifdef IOT_NODE_OUTPUT

namespace IotNode::Services::Output
{
  auto service0 = makeService(&Utils::Output::output0, 0);

  void setup()
  {
    Utils::UDP::instance.addService(&service0);
  }
}

#endif
