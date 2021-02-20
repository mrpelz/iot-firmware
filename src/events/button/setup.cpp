#include "./setup.h"

namespace IotNode {

namespace Button {
  Class buttons(config);

  void task(void * parameter) {
    for(;;) {
      buttons.update();
      yield();
    }
  }

  Class *setup(UDP::Class *udp, Relais *relais) {
    buttons.setDebug(Log::debug);

    buttons.setChangeCallback([udp, relais](Button::Update update) {
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
        relais->toggle();
      }
    });

    buttons.start();

    xTaskCreate(task, "button_maintenance", 10000, NULL, 1, NULL);

    return &buttons;
  }
}

} // project namespace
