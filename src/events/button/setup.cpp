#include "./setup.h"

namespace IotNode {
namespace Events {

namespace Button {
  void setup(Utils::UDP::Class *udp) {
    auto event0 = makeEvent(udp, 0);

    Utils::Button::button0.setChangeCallback([udp, event0](Utils::Button::Update update) {
      if (udp->isListening() && udp->hasEventPeer()) {
        event0(update);
        return;
      }

      Utils::Log::debug("info.buttons.change-callback", "udp event not usable");
      if (update.downChanged && !update.down) {
        Utils::Log::debug("info.buttons.change-callback", "triggering override");
        Utils::Relais::relais0.toggle();
      }
    });
  }
}

} // section namespace
} // project namespace
