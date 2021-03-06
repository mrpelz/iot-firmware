#include "./setup.h"

namespace IotNode {

namespace Relais {
  Relais::Class relais0({ 4, true });
  auto service0 = makeService(&relais0, 0);

  void setup(UDP::Class *udp, Button::Class *buttons) {
    udp->addService(&service0);

    buttons->setChangeCallback([udp](Button::Update update) {
      if (udp->isListening() && udp->hasEventPeer()) {
        buttonEvent(udp, update);
        return;
      }

      Log::debug("info.buttons.change-callback", "udp event not usable");

      if (
        update.index == 0
        && update.downChanged
        && !update.down
      ) {
        Log::debug("info.buttons.change-callback", "triggering override");
        relais0.toggle();
      }
    });

    relais0.init();
  }
}

} // project namespace
