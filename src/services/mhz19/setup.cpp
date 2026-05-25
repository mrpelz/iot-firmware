#ifdef IOT_NODE_MHZ19

#include "./setup.h"

namespace IotNode::Services::Mhz19
{
  Utils::UDP::Service service = {
      .serviceId = ids::mhz19,
      .serviceIndex = 0,
      .handler = handler,
  };

  void setup()
  {
    initializer();

    Utils::UDP::instance.addService(&service);
  }
}

#endif
