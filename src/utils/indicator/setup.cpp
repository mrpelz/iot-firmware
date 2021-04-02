#include "./setup.h"

#ifdef IOT_NODE_INDICATORS

namespace IotNode {
namespace Utils {

namespace Indicator {
  Class indicator0({
    #if defined(IOT_NODE_BOARD_ROOM_SENSOR) || defined(IOT_NODE_BOARD_TEST_DEVICE)
      IOT_NODE_WT32_ETH01_TXD_LED,
      true,
    #elif defined(IOT_NODE_BOARD_OBI_JACK)
      IOT_NODE_OBI_JACK_STATUS_LED,
      false,
    #endif
    INDICATOR_BLINK_PERIOD_ON,
    INDICATOR_BLINK_PERIOD_OFF
  });

  void update() {
    indicator0.update();
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
    indicator0.init();

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
