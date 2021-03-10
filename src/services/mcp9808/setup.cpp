#ifdef IOT_NODE_MCP9808

#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Mcp9808 {
  Utils::UDP::Service service = {
    .serviceId = ids::mcp9808,
    .handler = handler,
  };

  void setup(Utils::UDP::Class *udp, TwoWire *i2c) {
    initializer(i2c);

    udp->addService(&service);
  }
}

} // section namespace
} // project namespace

#endif
