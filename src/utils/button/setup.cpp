#include "./setup.h"

#ifdef IOT_NODE_BUTTONS

namespace IotNode {
namespace Utils {

namespace Button {
  #ifdef IOT_NODE_BOARD_SHELLY1
    Class button0(button0Config);
  #endif

  void update() {
    #ifdef IOT_NODE_BOARD_SHELLY1
      button0.update();
    #endif
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
    #ifdef IOT_NODE_BOARD_SHELLY1
      button0.setDebug(Log::debug);
      button0.start();
    #endif

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

#endif
