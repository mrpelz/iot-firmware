#include "./setup.h"

namespace IotNode {
namespace Utils {

namespace Relais {
  Class relais0({ 4, true });
  Class relais1({ 32, true });

  void setup(UDP::Class *udp, Events::Button::Class *buttons) {
    buttons->setChangeCallback([udp](Events::Button::Update update) {
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
    relais1.init();
  }
}

} // section namespace
} // project namespace
