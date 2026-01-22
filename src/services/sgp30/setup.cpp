#ifdef IOT_NODE_SGP30

#include "./setup.h"

namespace IotNode
{
  namespace Services
  {

    namespace Sgp30
    {
      Utils::UDP::Service service = {
          .serviceId = ids::sgp30,
          .serviceIndex = 0,
          .handler = handler,
      };

      void setup()
      {
        initializer(&Utils::I2C::bus);

        Utils::UDP::instance.addService(&service);
      }
    }

  } // section namespace
} // project namespace

#endif
