#include "./setup.h"

namespace IotNode {

namespace Button {
  Class buttons(config);
  const TickType_t delay = 10 / portTICK_PERIOD_MS;

  void task(void * parameter) {
    for(;;) {
      buttons.update();
      vTaskDelay(delay);
    }
  }

  Class *setup() {
    buttons.setDebug(Log::debug);

    buttons.start();

    xTaskCreate(task, "button_maintenance", 2048, NULL, 3, NULL);

    return &buttons;
  }
}

} // project namespace
