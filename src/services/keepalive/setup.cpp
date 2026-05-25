#include "./setup.h"

namespace IotNode::Services::Keepalive
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
