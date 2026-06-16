#ifdef IOT_NODE_BOARD_OBI_JACK

#include "./button.h"

namespace IotNode::Setup::Button
{
  static const ::IotNode::Utils::Button::Config button0Config = {
      .pin = 14,
      .invert = false,
      .pullupEnable = true,
      .debounceTime = 50,
      .repeatTime = 3000,
      .longpressTime = 125,
      .noiseGateTime = 0,
  };

  ::IotNode::Utils::Button::Class button0(button0Config);

  void update()
  {
    button0.update();
  }

  void setup()
  {
    auto event0 = ::IotNode::Events::Button::makeEvent(&::IotNode::Utils::UDP::instance, 0);

    button0.setChangeCallback([event0](::IotNode::Utils::Button::Update update)
                              {
      if (::IotNode::Utils::UDP::instance.isListening() && ::IotNode::Utils::UDP::instance.hasEventPeer()) {
        event0(update);
        return;
      }

      if (update.downChanged && !update.down) {
        Output::output0.toggle();
      } });

    button0.start();
  }
}

#endif
