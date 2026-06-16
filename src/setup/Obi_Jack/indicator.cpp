#ifdef IOT_NODE_BOARD_OBI_JACK

#include "./indicator.h"

namespace IotNode::Setup::Indicator
{
  ::IotNode::Utils::Indicator::ClassPin indicator0({4,
                                                    false,
                                                    INDICATOR_BLINK_PERIOD_ON,
                                                    INDICATOR_BLINK_PERIOD_OFF});

  auto service0 = ::IotNode::Services::Indicator::makeService(&indicator0, 0);

  void update()
  {
    indicator0.update();
  }

  void setup()
  {
    indicator0.init();
    indicator0.setOn(true);

    ::IotNode::Utils::UDP::instance.addService(&service0);
  }
}

#endif
