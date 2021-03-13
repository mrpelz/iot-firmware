#include "./setup.h"

namespace IotNode {
namespace Utils {

namespace Button {
  Class button0(button0Config);

  void update() {
    button0.update();
  }

  #ifdef ARDUINO_ARCH_ESP32
    void task(void *parameter) {
      for(;;) {
        update();
        vTaskDelay(10 / portTICK_PERIOD_MS);
      }
    }
  #endif

  void setup() {
    button0.start();

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
  }
}

} // section namespace
} // project namespace
