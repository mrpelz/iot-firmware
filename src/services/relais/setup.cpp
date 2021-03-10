#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Relais {
  auto service0 = makeService(&Utils::Relais::relais0, 0);

  void setup(Utils::UDP::Class *udp) {
    udp->addService(&service0);
  }
}

} // section namespace
} // project namespace
