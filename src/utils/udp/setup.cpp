#include "./setup.h"

namespace IotNode {
namespace Utils {

namespace UDP {
  Class udp(8266);

  Class *setup() {
    udp.setDebug(Log::debug);

    return &udp;
  }
}

} // section namespace
} // project namespace