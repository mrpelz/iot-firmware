#include "./setup.h"

#ifdef IOT_NODE_BUTTONS

namespace IotNode
{
  namespace Utils
  {

    namespace Button
    {
      Class button0(button0Config);

#if defined(IOT_NODE_BOARD_SHELLYI3) || defined(IOT_NODE_BOARD_ESP_NOW_TEST_BUTTON) || defined(IOT_NODE_BOARD_ESP_NOW_TEST_BUTTON_II)
      Class button1(button1Config);
#endif

#ifdef IOT_NODE_BOARD_SHELLYI3
      Class button2(button2Config);
#endif

      void update()
      {
#ifdef IOT_NODE_BOARD_ESP_NOW_TEST_BUTTON_II
        // implement i2c io-expander readout and button value injection here
#else
        button0.update();

#if defined(IOT_NODE_BOARD_SHELLYI3) || defined(IOT_NODE_BOARD_ESP_NOW_TEST_BUTTON)
        button1.update();
#endif

#ifdef IOT_NODE_BOARD_SHELLYI3
        button2.update();
#endif
#endif
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
#ifdef IOT_NODE_BOARD_ESP_NOW_TEST_BUTTON_II
        // implement i2c io-expander setup here
#endif

        button0.start();

#if defined(IOT_NODE_BOARD_SHELLYI3) || defined(IOT_NODE_BOARD_ESP_NOW_TEST_BUTTON) || defined(IOT_NODE_BOARD_ESP_NOW_TEST_BUTTON_II)
        button1.start();
#endif

#ifdef IOT_NODE_BOARD_SHELLYI3
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
            CONFIG_ARDUINO_RUNNING_CORE);
#endif
      }
    }

  } // section namespace
} // project namespace

#endif
