#ifdef IOT_NODE_ESP32

#include "./setup.h"

namespace IotNode::Services::Async
{
  Utils::UDP::Service service = {
      .serviceId = ids::async,
      .serviceIndex = 0,
      .handler = handler,
  };

  void setup()
  {
    Utils::UDP::instance.addService(&service);

    initializer();
  }
}

#endif
