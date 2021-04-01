#include "./setup.h"

namespace IotNode {
namespace Utils {

namespace Button {
  // Class button0(button0Config);

  void update() {
    // button0.update();
  }

  #ifdef IOT_NODE_ESP32
    void task(void *parameter) {
      for(;;) {
        update();
        vTaskDelay(IOT_NODE_MUTLITASKING_DELAY / portTICK_PERIOD_MS);
      }
    }
  #endif

  void setup() {
    // button0.setDebug(Log::debug);
    // button0.start();

    #ifdef IOT_NODE_ESP32
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
