#include "./setup.h"

#ifdef IOT_NODE_INPUT

namespace IotNode {
namespace Events {

namespace Input {
  Class input0(39);

  void update() {
    input0.update();
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
    auto event0 = makeEvent(&Utils::UDP::instance, 0);
    input0.setChangeCallback(event0);

    input0.start();

    #ifdef IOT_NODE_ESP32
      xTaskCreatePinnedToCore(
        task,
        "input_maintenance",
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
