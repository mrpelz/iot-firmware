#include "./setup.h"

#ifdef IOT_NODE_INDICATORS

namespace IotNode {
namespace Utils {

namespace Indicator {
  #if defined(IOT_NODE_BOARD_ROOM_SENSOR) || defined(IOT_NODE_BOARD_TEST_DEVICE)
    Class indicator0({
      IOT_NODE_WT32_ETH01_TXD_LED,
      true,
      INDICATOR_BLINK_PERIOD_ON,
      INDICATOR_BLINK_PERIOD_OFF
    });
  #endif

  void update() {
    #if defined(IOT_NODE_BOARD_ROOM_SENSOR) || defined(IOT_NODE_BOARD_TEST_DEVICE)
      indicator0.update();
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
      indicator0.init();
    #endif

    #ifdef IOT_NODE_ESP32
      xTaskCreatePinnedToCore(
        task,
        "indicator_maintenance",
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
