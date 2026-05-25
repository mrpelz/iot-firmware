#ifdef IOT_NODE_CCS811

#include "./setup.h"

namespace IotNode::Services::Ccs811
{
  Utils::UDP::Service service = {
      .serviceId = ids::ccs811,
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
