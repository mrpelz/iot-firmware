#include "./setup.h"

namespace IotNode
{
  namespace Services
  {

    namespace Keepalive
    {
      Utils::UDP::Service service = {
          .serviceId = ids::keepalive,
          .serviceIndex = 0,
          .handler = makeHandler(),
      };

      void setup()
      {
        Utils::UDP::instance.addService(&service);
      }
    }

  } // section namespace
} // project namespace
