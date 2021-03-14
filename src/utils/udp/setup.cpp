#include "./setup.h"

namespace IotNode {
namespace Utils {

namespace UDP {
  Class instance(8266);

  void setup() {
    instance.setDebug(Log::debug);
  }
}

} // section namespace
} // project namespace