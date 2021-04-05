#include "./setup.h"

#ifdef IOT_NODE_BUTTONS

namespace IotNode {
namespace Events {

namespace Button {
  void setup() {
    auto event0 = makeEvent(&Utils::UDP::instance, 0);

    #ifdef IOT_NODE_RELAIS
      Utils::Button::button0.setChangeCallback([event0](Utils::Button::Update update) {
        if (Utils::UDP::instance.isListening() && Utils::UDP::instance.hasEventPeer()) {
          event0(update);
          return;
        }

        Utils::Log::debug("info.buttons.change-callback", "udp event not usable");
        if (update.downChanged && !update.down) {
          Utils::Log::debug("info.buttons.change-callback", "triggering override");
          Utils::Relais::relais0.toggle();
        }
      });
    #else
      Utils::Button::button0.setChangeCallback(event0);
    #endif

    #ifdef IOT_NODE_BOARD_SHELLYI3
      auto event1 = makeEvent(&Utils::UDP::instance, 1);
      Utils::Button::button1.setChangeCallback(event1);

      auto event2 = makeEvent(&Utils::UDP::instance, 2);
      Utils::Button::button2.setChangeCallback(event2);
    #endif
  }
}

} // section namespace
} // project namespace

#endif
