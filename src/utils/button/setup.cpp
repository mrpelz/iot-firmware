#include "./setup.h"

#ifdef IOT_NODE_BUTTONS

namespace IotNode {
namespace Utils {

namespace Button {
  Class button0(button0Config);

  #ifdef IOT_NODE_BOARD_SHELLYI3
    Class button1(button1Config);
    Class button2(button2Config);
  #endif

  void update() {
    button0.update();

    #ifdef IOT_NODE_BOARD_SHELLYI3
      button1.update();
      button2.update();
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
    button0.start();

    #ifdef IOT_NODE_BOARD_SHELLYI3
      button1.start();
      button2.start();
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
