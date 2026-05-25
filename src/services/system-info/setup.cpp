#include "./setup.h"

namespace IotNode::Services::SystemInfo
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
