#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Indicator {
  Utils::UDP::Service service = {
    .serviceId = serviceIds::blink,
    .handler = handler,
  };

  void setup(Utils::UDP::Class *udp) {
    udp->addService(&service);
  }
}

} // section namespace
} // project namespace
