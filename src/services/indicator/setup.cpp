#include "./setup.h"

#ifdef IOT_NODE_INDICATORS

namespace IotNode
{
  namespace Services
  {

    namespace Indicator
    {
      auto service0 = makeService(&Utils::Indicator::indicator0, 0);

#if defined(IOT_NODE_BOARD_ROOM_SENSOR) || defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO)
      auto service1 = makeService(&Utils::Indicator::indicator1, 1);
      auto service2 = makeService(&Utils::Indicator::indicator2, 2);
#endif

      void setup()
      {
        Utils::UDP::instance.addService(&service0);

#if defined(IOT_NODE_BOARD_ROOM_SENSOR) || defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO)
        Utils::UDP::instance.addService(&service1);
        Utils::UDP::instance.addService(&service2);
#endif
      }
    }

  } // section namespace
} // project namespace

#endif
