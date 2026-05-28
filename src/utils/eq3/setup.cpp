#include "./setup.h"

#ifdef IOT_NODE_EQ3

namespace IotNode::Utils::EQ3
{
  void update()
  {
  }

#ifdef IOT_NODE_ESP32
  void task(void *parameter)
  {
    for (;;)
    {
      update();
      vTaskDelay(IOT_NODE_MUTLITASKING_DELAY / portTICK_PERIOD_MS);
    }
  }
#endif

  void setup()
  {
#ifdef IOT_NODE_ESP32
    xTaskCreatePinnedToCore(
        task,
        "eq3_maintenance",
        FREERTOS_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        NULL,
        CONFIG_ARDUINO_RUNNING_CORE);
#endif
  }
}

#endif
