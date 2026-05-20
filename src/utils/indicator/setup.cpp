#include "./setup.h"

#ifdef IOT_NODE_INDICATORS

namespace IotNode
{
  namespace Utils
  {

    namespace Indicator
    {
#ifndef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
#ifdef IOT_NODE_BOARD_ROOM_SENSOR
      ClassExpander indicator0({
        ROOM_SENSOR_INDICATOR_LED_R,
            true,
#else
      ClassPin indicator0({
#if defined(IOT_NODE_BOARD_TEST_DEVICE)
          17,
          true,
#elif defined(IOT_NODE_BOARD_OBI_JACK)
          4,
          false,
#elif defined(IOT_NODE_BOARD_H801)
          5,
          true,
#elif defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V13) || defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V1)
          13,
          true,
#elif defined(IOT_NODE_BOARD_SHELLYPLUS1)
          0,
          true,
#endif
#endif
            INDICATOR_BLINK_PERIOD_ON,
            INDICATOR_BLINK_PERIOD_OFF
      });
#endif

#ifdef IOT_NODE_BOARD_ROOM_SENSOR
      ClassExpander indicator1({ROOM_SENSOR_INDICATOR_LED_G,
                                true});

      ClassExpander indicator2({ROOM_SENSOR_INDICATOR_LED_B,
                                true});

      ClassPin indicatorNotInUse0({5,
                                   true,
                                   INDICATOR_BLINK_PERIOD_ON,
                                   INDICATOR_BLINK_PERIOD_OFF});

      ClassPin indicatorNotInUse1({17,
                                   true,
                                   INDICATOR_BLINK_PERIOD_ON,
                                   INDICATOR_BLINK_PERIOD_OFF});
#endif

#ifdef IOT_NODE_BOARD_H801
      ClassPin indicator1({1,
                           true,
                           INDICATOR_BLINK_PERIOD_ON,
                           INDICATOR_BLINK_PERIOD_OFF});
#endif

      void update()
      {
#ifndef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
        indicator0.update();
#endif

#ifdef IOT_NODE_BOARD_ROOM_SENSOR
        indicator1.update();
        indicator2.update();
#endif

#ifdef IOT_NODE_BOARD_H801
        indicator1.update();
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
#ifndef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
        indicator0.init();
#endif

#if defined(IOT_NODE_BOARD_ROOM_SENSOR)
        indicator1.init();
        indicator2.init();

        indicatorNotInUse0.init();
        indicatorNotInUse1.init();
#endif

#ifdef IOT_NODE_BOARD_H801
        indicator1.init();
#endif

#ifdef IOT_NODE_ESP32
        xTaskCreatePinnedToCore(
            task,
            "indicator_maintenance",
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
