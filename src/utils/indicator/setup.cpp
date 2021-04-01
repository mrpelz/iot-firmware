#include "./setup.h"

namespace IotNode {
namespace Utils {

namespace Indicator {
  Class rxdLed({
    WT32_ETH01_RXD_LED,
    true,
    INDICATOR_BLINK_PERIOD_ON,
    INDICATOR_BLINK_PERIOD_OFF
  });

  Class txdLed({
    WT32_ETH01_TXD_LED,
    true,
    INDICATOR_BLINK_PERIOD_ON,
    INDICATOR_BLINK_PERIOD_OFF
  });

  void update() {
    rxdLed.update();
    txdLed.update();
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
    rxdLed.init();
    txdLed.init();

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
