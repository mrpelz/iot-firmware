#ifdef IOT_NODE_BOARD_H801

#include "./output-ng.h"

namespace IotNode::Setup::OutputNg
{
  ::IotNode::Utils::OutputNg::Dimmable indicatorR(5, true);
  auto serviceIndicatorR = ::IotNode::Services::OutputNg::makeService(&indicatorR, &::IotNode::Utils::UDP::instance, 0);

  ::IotNode::Utils::OutputNg::Dimmable indicatorG(1, true);
  auto serviceIndicatorG = ::IotNode::Services::OutputNg::makeService(&indicatorG, &::IotNode::Utils::UDP::instance, 1);

  ::IotNode::Utils::OutputNg::Dimmable ledR(15, false);
  auto serviceLedR = ::IotNode::Services::OutputNg::makeService(&ledR, &::IotNode::Utils::UDP::instance, 2);

  ::IotNode::Utils::OutputNg::Dimmable ledG(12, false);
  auto serviceLedG = ::IotNode::Services::OutputNg::makeService(&ledG, &::IotNode::Utils::UDP::instance, 4);

  ::IotNode::Utils::OutputNg::Dimmable ledB(13, false);
  auto serviceLedB = ::IotNode::Services::OutputNg::makeService(&ledB, &::IotNode::Utils::UDP::instance, 3);

  ::IotNode::Utils::OutputNg::Dimmable ledW1(14, false);
  auto serviceLedW1 = ::IotNode::Services::OutputNg::makeService(&ledW1, &::IotNode::Utils::UDP::instance, 5);

  ::IotNode::Utils::OutputNg::Dimmable ledW2(4, false);
  auto serviceLedW2 = ::IotNode::Services::OutputNg::makeService(&ledW2, &::IotNode::Utils::UDP::instance, 6);

  void update()
  {
    indicatorR.update();
    indicatorG.update();

    ledR.update();
    ledG.update();
    ledB.update();
    ledW1.update();
    ledW2.update();
  }

  void setup()
  {
    indicatorR.init();
    ::IotNode::Utils::UDP::instance.addService(&serviceIndicatorR);
    indicatorG.init();
    ::IotNode::Utils::UDP::instance.addService(&serviceIndicatorG);

    ledR.init();
    ::IotNode::Utils::UDP::instance.addService(&serviceLedR);
    ledG.init();
    ::IotNode::Utils::UDP::instance.addService(&serviceLedG);
    ledB.init();
    ::IotNode::Utils::UDP::instance.addService(&serviceLedB);
    ledW1.init();
    ::IotNode::Utils::UDP::instance.addService(&serviceLedW1);
    ledW2.init();
    ::IotNode::Utils::UDP::instance.addService(&serviceLedW2);
  }
}

#endif
