#include "./setup.h"

#ifdef IOT_NODE_VCC

namespace IotNode::Events::VCC
{
  ChangeCallback cb = [](unsigned short vcc) {};

  auto keepalive = Utils::Keepalive::Class(
      1000,
      []()
      {
        cb(Utils::VCC::get());
      });

  void update()
  {
    keepalive.update();
  }

#ifdef IOT_NODE_ESP_NOW_NODE
  void setupEspNow()
  {
    cb = makeEspNowEvent();
    keepalive.start();
  }
#endif
}

#endif
