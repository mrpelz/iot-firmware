#include "./setup.h"

namespace IotNode {
namespace Utils {

namespace Relais {
  Class relais0({ 4, true });
  Class relais1({ 32, true });

  void setup() {
    relais0.init();
    relais1.init();
  }
}

} // section namespace
} // project namespace
