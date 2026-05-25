#ifdef IOT_NODE_TSL2561

#include "./setup.h"

namespace IotNode::Services::Tsl2561
{
  Utils::UDP::Service service = {
      .serviceId = ids::tsl2561,
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
