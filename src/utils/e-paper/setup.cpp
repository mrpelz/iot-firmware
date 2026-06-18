#include "./setup.h"

#ifdef IOT_NODE_E_PAPER

namespace IotNode::Utils::EPaper
{
#ifdef IOT_NODE_ESP32
  void task(void *parameter)
  {
    websocketStart();

    for (;;)
    {
      websocketTouch();

      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
#endif

  void setup()
  {
    bool touchInit = touchSetup();
    if (!touchInit)
    {
      return;
    }

    bool epdInit = epdSetup();
    if (!epdInit)
    {
      return;
    }

#ifdef IOT_NODE_ESP32
    xTaskCreatePinnedToCore(
        task,
        "e-paper_maintenance",
        FREERTOS_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY,
        NULL,
        CONFIG_ARDUINO_RUNNING_CORE);
#endif
  }
}

#endif
