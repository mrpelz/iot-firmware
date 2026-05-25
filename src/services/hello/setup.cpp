#include "./setup.h"

namespace IotNode::Services::Hello
{
  Utils::UDP::Service service = {
      .serviceId = ids::hello,
      .serviceIndex = 0,
      .handler = handler,
  };

  void setup()
  {
    Utils::UDP::instance.addService(&service);
  }
}
