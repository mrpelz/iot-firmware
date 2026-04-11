#include "./setup.h"

#ifdef IOT_NODE_HMMD_MOTION

namespace IotNode
{
  namespace Events
  {

    namespace HmmdMotion
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

  } // section namespace
} // project namespace

#endif
