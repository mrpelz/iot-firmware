#ifdef IOT_NODE_SGP30

#include "./setup.h"

namespace IotNode::Services::Sgp30
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

#endif
