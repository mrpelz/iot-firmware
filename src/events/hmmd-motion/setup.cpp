#include "./setup.h"

#ifdef IOT_NODE_HMMD_MOTION

namespace IotNode::Events::HmmdMotion
{
  void update()
  {
    checkForTargets();
  }

  void setup()
  {
    initializer();
  }
}

#endif
