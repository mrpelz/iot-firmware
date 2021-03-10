#include "./setup.h"

namespace IotNode {
namespace Events {

namespace Button {
  Class buttons(config);

  void update() {
    buttons.update();
  }

  #ifdef ARDUINO_ARCH_ESP32
    void task(void *parameter) {
      for(;;) {
        update();
        vTaskDelay(10 / portTICK_PERIOD_MS);
      }
    }
  #endif

  Class *setup() {
    buttons.setDebug(Utils::Log::debug);

    buttons.start();

    #ifdef ARDUINO_ARCH_ESP32
      xTaskCreatePinnedToCore(
        task,
        "button_maintenance",
        2048,
        NULL,
        3,
        NULL,
        CONFIG_ARDUINO_RUNNING_CORE
      );
    #endif

    return &buttons;
  }
}

} // section namespace
} // project namespace
