#include "./setup.h"

namespace IotNode {

namespace UDP {
  Class udp(8266);

  Class *setup() {
    udp.setDebug(Log::debug);

    return &udp;
  }
}

} // project namespace