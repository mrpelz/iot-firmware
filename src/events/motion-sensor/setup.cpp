#include "./setup.h"

#ifdef IOT_NODE_MOTION_SENSOR

namespace IotNode {
namespace Events {

namespace MotionSensor {
  #if defined(IOT_NODE_BOARD_ROOM_SENSOR) || defined(IOT_NODE_BOARD_TEST_DEVICE)
    Class motionSensor0(39);
  #endif

  void update() {
    #if defined(IOT_NODE_BOARD_ROOM_SENSOR) || defined(IOT_NODE_BOARD_TEST_DEVICE)
      motionSensor0.update();
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
    #if defined(IOT_NODE_BOARD_ROOM_SENSOR) || defined(IOT_NODE_BOARD_TEST_DEVICE)
      motionSensor0.setDebug(Utils::Log::debug);

      auto event0 = makeEvent(&Utils::UDP::instance, 0);
      motionSensor0.setChangeCallback(event0);

      motionSensor0.start();
    #endif

    #ifdef IOT_NODE_ESP32
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

#endif
