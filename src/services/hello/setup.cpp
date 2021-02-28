#include "./setup.h"

namespace IotNode {

namespace Hello {
  UDP::Service service = {
  .serviceId = serviceIds::hello,
  .handler = handler,
};

  void setup(UDP::Class *udp) {
    udp->addService(&service);
  }
}

} // project namespace
