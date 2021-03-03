#ifdef IOT_NODE_MCP9808

#include "./setup.h"

namespace IotNode {

namespace Mcp9808 {
  UDP::Service service = {
    .serviceId = serviceIds::mcp9808,
    .handler = handler,
  };

  void setup(UDP::Class *udp, TwoWire *i2c) {
    initializer(i2c);

    udp->addService(&service);
  }
}

} // project namespace

#endif
