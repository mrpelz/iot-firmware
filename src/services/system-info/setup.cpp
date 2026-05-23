#include "./setup.h"

namespace IotNode
{
  namespace Services
  {

    namespace SystemInfo
    {
      Utils::UDP::Service service = {
          .serviceId = ids::systemInfo,
          .serviceIndex = 0,
          .handler = handler,
      };

      void setup()
      {
        Utils::UDP::instance.addService(&service);
      }
    }

  } // section namespace
} // project namespace
