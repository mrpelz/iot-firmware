#include "./setup.h"

namespace IotNode {
namespace Events {

namespace MotionSensor {
  // Class motionSensor0(36);
  // Class motionSensor1(39);

  void update() {
    // motionSensor0.update();
    // motionSensor1.update();
  }

  #ifdef ARDUINO_ARCH_ESP32
    void task(void *parameter) {
      for(;;) {
        update();
        vTaskDelay(IOT_NODE_MUTLITASKING_DELAY / portTICK_PERIOD_MS);
      }
    }
  #endif

  void setup() {
    // motionSensor0.setDebug(Utils::Log::debug);
    // motionSensor1.setDebug(Utils::Log::debug);

    // auto event0 = makeEvent(&Utils::UDP::instance, 0);
    // auto event1 = makeEvent(&Utils::UDP::instance, 1);
    // motionSensor0.setChangeCallback(event0);
    // motionSensor1.setChangeCallback(event1);

    // motionSensor0.start();
    // motionSensor1.start();

    #ifdef ARDUINO_ARCH_ESP32
      xTaskCreatePinnedToCore(
        task,
        "motionSensor_maintenance",
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
