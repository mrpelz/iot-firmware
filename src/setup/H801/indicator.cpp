#ifdef IOT_NODE_BOARD_H801

#include "./indicator.h"

namespace IotNode::Setup::Indicator
{
  ::IotNode::Utils::Indicator::ClassPin indicator0({5,
                                                    true,
                                                    INDICATOR_BLINK_PERIOD_ON,
                                                    INDICATOR_BLINK_PERIOD_OFF});

  ::IotNode::Utils::Indicator::ClassPin indicator1({1,
                                                    true,
                                                    INDICATOR_BLINK_PERIOD_ON,
                                                    INDICATOR_BLINK_PERIOD_OFF});

  auto service0 = ::IotNode::Services::Indicator::makeService(&indicator0, 0);

  void update()
  {
    indicator0.update();
    indicator1.update();
  }

  void setup()
  {
    indicator0.init();
    indicator0.setOn(true);

    indicator1.init();

    ::IotNode::Utils::UDP::instance.addService(&service0);
  }
}

#endif
