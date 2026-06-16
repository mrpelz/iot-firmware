#ifdef IOT_NODE_BOARD_SHELLYI3

#include "./button.h"

namespace IotNode::Setup::Button
{
  static const ::IotNode::Utils::Button::Config button0Config = {
      .pin = 14,
      .invert = true,
      .pullupEnable = false,
      .debounceTime = 50,
      .repeatTime = 3000,
      .longpressTime = 125,
      .noiseGateTime = 50,
  };

  static const ::IotNode::Utils::Button::Config button1Config = {
      .pin = 12,
      .invert = true,
      .pullupEnable = false,
      .debounceTime = 50,
      .repeatTime = 3000,
      .longpressTime = 125,
      .noiseGateTime = 50,
  };

  static const ::IotNode::Utils::Button::Config button2Config = {
      .pin = 13,
      .invert = true,
      .pullupEnable = false,
      .debounceTime = 50,
      .repeatTime = 3000,
      .longpressTime = 125,
      .noiseGateTime = 50,
  };

  ::IotNode::Utils::Button::Class button0(button0Config);
  ::IotNode::Utils::Button::Class button1(button1Config);
  ::IotNode::Utils::Button::Class button2(button2Config);

  void update()
  {
    button0.update();
    button1.update();
    button2.update();
  }

  void setup()
  {
    auto event0 = ::IotNode::Events::Button::makeEvent(&::IotNode::Utils::UDP::instance, 0);
    auto event1 = ::IotNode::Events::Button::makeEvent(&::IotNode::Utils::UDP::instance, 1);
    auto event2 = ::IotNode::Events::Button::makeEvent(&::IotNode::Utils::UDP::instance, 2);

    button0.setChangeCallback(event0);
    button1.setChangeCallback(event1);
    button2.setChangeCallback(event2);

    button0.start();
    button1.start();
    button2.start();
  }
}

#endif
