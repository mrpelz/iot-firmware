#include "./setup.h"

#ifdef IOT_NODE_LED

namespace IotNode
{
  namespace Utils
  {

    namespace Led
    {
#ifdef IOT_NODE_BOARD_H801
      FadeLed led0(15);
      FadeLed led1(13);
      FadeLed led2(12);
      FadeLed led3(14);
      FadeLed led4(4);
#endif

      void update()
      {
        FadeLed::update();
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
#ifdef IOT_NODE_BOARD_H801
#define IOT_NODE_LED_FADE_TIME 300

        led0.setTime(IOT_NODE_LED_FADE_TIME);
        led1.setTime(IOT_NODE_LED_FADE_TIME);
        led2.setTime(IOT_NODE_LED_FADE_TIME);
        led3.setTime(IOT_NODE_LED_FADE_TIME);
        led4.setTime(IOT_NODE_LED_FADE_TIME);

        led0.begin(0);
        led1.begin(0);
        led2.begin(0);
        led3.begin(0);
        led4.begin(0);

#endif

        FadeLed::update();

#ifdef IOT_NODE_ESP32
        xTaskCreatePinnedToCore(
            task,
            "led_maintenance",
            2048,
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
