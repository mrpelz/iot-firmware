#include "./setup.h"

#ifdef IOT_NODE_LED

namespace IotNode
{
  namespace Services
  {

    namespace Led
    {
#ifdef IOT_NODE_BOARD_H801
      auto service0 = makeService(&Utils::Led::led0, 0);
      auto service1 = makeService(&Utils::Led::led1, 1);
      auto service2 = makeService(&Utils::Led::led2, 2);
      auto service3 = makeService(&Utils::Led::led3, 3);
      auto service4 = makeService(&Utils::Led::led4, 4);
#endif

      void setup()
      {
#ifdef IOT_NODE_BOARD_H801
        Utils::UDP::instance.addService(&service0);
        Utils::UDP::instance.addService(&service1);
        Utils::UDP::instance.addService(&service2);
        Utils::UDP::instance.addService(&service3);
        Utils::UDP::instance.addService(&service4);
#endif
      }
    }

  } // section namespace
} // project namespace

#endif
