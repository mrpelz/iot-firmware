#ifdef IOT_NODE_BOARD_H801

#include "./led.h"

namespace IotNode::Setup::Led
{
  FadeLed led0(15);
  FadeLed led1(13);
  FadeLed led2(12);
  FadeLed led3(14);
  FadeLed led4(4);

  auto service0 = ::IotNode::Services::Led::makeService(&led0, 0);
  auto service1 = ::IotNode::Services::Led::makeService(&led1, 1);
  auto service2 = ::IotNode::Services::Led::makeService(&led2, 2);
  auto service3 = ::IotNode::Services::Led::makeService(&led3, 3);
  auto service4 = ::IotNode::Services::Led::makeService(&led4, 4);

  void update()
  {
    FadeLed::update();
  }

  void setup()
  {
    led0.setTime(300);
    led1.setTime(300);
    led2.setTime(300);
    led3.setTime(300);
    led4.setTime(300);

    led0.begin(0);
    led1.begin(0);
    led2.begin(0);
    led3.begin(0);
    led4.begin(0);

    FadeLed::update();

    ::IotNode::Utils::UDP::instance.addService(&service0);
    ::IotNode::Utils::UDP::instance.addService(&service1);
    ::IotNode::Utils::UDP::instance.addService(&service2);
    ::IotNode::Utils::UDP::instance.addService(&service3);
    ::IotNode::Utils::UDP::instance.addService(&service4);
  }
}

#endif
