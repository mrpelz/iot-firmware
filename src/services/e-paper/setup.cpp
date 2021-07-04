#include "./setup.h"

#ifdef IOT_NODE_E_PAPER

namespace IotNode {
namespace Services {

namespace EPaper {
  Utils::UDP::Service service = {
    .serviceId = ids::ePaper,
    .handler = makeHandler(),
  };

  void setup() {
    Utils::UDP::instance.addService(&service);
  }
}

} // section namespace
} // project namespace

#endif
