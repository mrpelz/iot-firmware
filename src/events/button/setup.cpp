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

  Class *setup() {
    buttons.setDebug(Log::debug);

    buttons.start();

    xTaskCreate(task, "button_maintenance", 10000, NULL, 3, NULL);

    return &buttons;
  }
}

} // project namespace
