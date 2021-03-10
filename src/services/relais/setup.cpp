#include "./setup.h"

namespace IotNode {
namespace Services {

namespace Relais {
  Class relais0({ 4, true });
  auto service0 = makeService(&relais0, 0);

  void setup(Utils::UDP::Class *udp, Events::Button::Class *buttons) {
    udp->addService(&service0);

    buttons->setChangeCallback([udp](Events::Button::Update update) {
      if (udp->isListening() && udp->hasEventPeer()) {
        buttonEvent(udp, update);
        return;
      }

      Utils::Log::debug("info.buttons.change-callback", "udp event not usable");

      if (
        update.index == 0
        && update.downChanged
        && !update.down
      ) {
        Utils::Log::debug("info.buttons.change-callback", "triggering override");
        relais0.toggle();
      }
    });

    relais0.init();
  }
}

} // section namespace
} // project namespace
