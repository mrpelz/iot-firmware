#include "./setup.h"

namespace IotNode {
namespace Services {

namespace SystemInfo {
  Utils::UDP::Service service = {
    .serviceId = ids::systemInfo,
    .handler = handler,
  };

  void setup(Utils::UDP::Class *udp) {
    udp->addService(&service);
  }
}

} // section namespace
} // project namespace
