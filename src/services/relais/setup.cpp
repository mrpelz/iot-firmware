#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Relais {
  auto service0 = makeService(&Utils::Relais::relais0, 0);
  auto service1 = makeService(&Utils::Relais::relais1, 1);

  void setup(Utils::UDP::Class *udp) {
    udp->addService(&service0);
    udp->addService(&service1);
  }
}

} // section namespace
} // project namespace
