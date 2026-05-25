#ifdef IOT_NODE_MCP9808

#include "./setup.h"

namespace IotNode::Services::Mcp9808
{
  Utils::UDP::Service service = {
      .serviceId = ids::mcp9808,
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
