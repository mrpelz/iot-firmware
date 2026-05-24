#include "./setup.h"

#ifdef IOT_NODE_E_PAPER

namespace IotNode
{
  namespace Utils
  {

    namespace EPaper
    {
#ifdef IOT_NODE_ESP32
      void task(void *parameter)
      {
        websocketStart();

        for (;;)
        {
          websocketTouch();

          vTaskDelay(IOT_NODE_MUTLITASKING_DELAY / portTICK_PERIOD_MS);
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

  } // section namespace
} // project namespace

#endif
