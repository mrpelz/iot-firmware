#include "./setup.h"

#ifdef IOT_NODE_INDICATORS

namespace IotNode
{
  namespace Services
  {

    namespace Indicator
    {
      auto service0 = makeService(&Utils::Indicator::indicator0, 0);

#ifdef IOT_NODE_BOARD_ROOM_SENSOR
      auto service1 = makeService(&Utils::Indicator::indicator1, 1);
      auto service2 = makeService(&Utils::Indicator::indicator2, 2);
#endif

      void setup()
      {
        Utils::UDP::instance.addService(&service0);

#ifdef IOT_NODE_BOARD_ROOM_SENSOR
        Utils::UDP::instance.addService(&service1);
        Utils::UDP::instance.addService(&service2);
#endif
      }
    }

  } // section namespace
} // project namespace

#endif
