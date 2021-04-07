#include "./setup.h"

namespace IotNode {
namespace Utils {

namespace UDP {
  Class instance(1337);

  void setup() {
    instance.setDebug(Log::debug);
  }
}

} // section namespace
} // project namespace
