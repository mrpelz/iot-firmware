#include "./setup.h"

namespace IotNode {
namespace Events {

namespace Button {
  void setup() {
    // auto event0 = makeEvent(&Utils::UDP::instance, 0);
    // Utils::Button::button0.setChangeCallback(event0);

    // Utils::Button::button0.setChangeCallback([event0](Utils::Button::Update update) {
    //   if (Utils::UDP::instance.isListening() && Utils::UDP::instance.hasEventPeer()) {
    //     event0(update);
    //     return;
    //   }

    //   Utils::Log::debug("info.buttons.change-callback", "udp event not usable");
    //   if (update.downChanged && !update.down) {
    //     Utils::Log::debug("info.buttons.change-callback", "triggering override");
    //     Utils::Relais::relais0.toggle();
    //   }
    // });
  }
}

} // section namespace
} // project namespace
