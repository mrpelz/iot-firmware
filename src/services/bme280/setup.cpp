#ifdef IOT_NODE_BME280

#include "./setup.h"

namespace IotNode
{
  namespace Services
  {

    namespace Bme280
    {
      Utils::UDP::Service service = {
          .serviceId = ids::bme280,
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
