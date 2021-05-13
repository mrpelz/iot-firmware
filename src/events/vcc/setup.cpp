#include "./setup.h"

#ifdef IOT_NODE_VCC

namespace IotNode {
namespace Events {

namespace VCC {
  ChangeCallback cb = [](uint16_t vcc) {};

  auto keepalive = Utils::Keepalive::Class(
    1000,
    []() {
      cb(Utils::VCC::get());
    }
  );

  void update() {
    keepalive.update();
  }

  #ifdef IOT_NODE_ESP_NOW_NODE
    void setupEspNow() {
      cb = makeEspNowEvent();
      keepalive.start();
    }
  #endif
}

} // section namespace
} // project namespace

#endif
