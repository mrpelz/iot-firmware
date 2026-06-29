#ifdef IOT_NODE_BOARD_H801

#include "./output-ng.h"

namespace IotNode::Setup::OutputNg
{
  ::IotNode::Utils::OutputNg::Dimmable indicator0(5, true);
  auto serviceIndicator0 = ::IotNode::Services::OutputNg::makeService(&indicator0, &::IotNode::Utils::UDP::instance, 0);

  ::IotNode::Utils::OutputNg::Dimmable indicator1(1, true);
  auto serviceIndicator1 = ::IotNode::Services::OutputNg::makeService(&indicator1, &::IotNode::Utils::UDP::instance, 1);

  ::IotNode::Utils::OutputNg::Dimmable led0(15, true);
  auto serviceLed0 = ::IotNode::Services::OutputNg::makeService(&led0, &::IotNode::Utils::UDP::instance, 0);

  ::IotNode::Utils::OutputNg::Dimmable led1(13, true);
  auto serviceLed1 = ::IotNode::Services::OutputNg::makeService(&led1, &::IotNode::Utils::UDP::instance, 1);

  ::IotNode::Utils::OutputNg::Dimmable led2(12, true);
  auto serviceLed2 = ::IotNode::Services::OutputNg::makeService(&led2, &::IotNode::Utils::UDP::instance, 2);

  ::IotNode::Utils::OutputNg::Dimmable led3(14, true);
  auto serviceLed3 = ::IotNode::Services::OutputNg::makeService(&led3, &::IotNode::Utils::UDP::instance, 3);

  ::IotNode::Utils::OutputNg::Dimmable led4(4, true);
  auto serviceLed4 = ::IotNode::Services::OutputNg::makeService(&led4, &::IotNode::Utils::UDP::instance, 4);

  void update()
  {
    indicator0.update();
    indicator1.update();

    led0.update();
    led1.update();
    led2.update();
    led3.update();
    led4.update();
  }

  void setup()
  {
    indicator0.init();
    ::IotNode::Utils::UDP::instance.addService(&serviceIndicator0);
    indicator1.init();
    ::IotNode::Utils::UDP::instance.addService(&serviceIndicator1);

    led0.init();
    ::IotNode::Utils::UDP::instance.addService(&serviceLed0);
    led1.init();
    ::IotNode::Utils::UDP::instance.addService(&serviceLed1);
    led2.init();
    ::IotNode::Utils::UDP::instance.addService(&serviceLed2);
    led3.init();
    ::IotNode::Utils::UDP::instance.addService(&serviceLed3);
    led4.init();
    ::IotNode::Utils::UDP::instance.addService(&serviceLed4);
  }
}

#endif
