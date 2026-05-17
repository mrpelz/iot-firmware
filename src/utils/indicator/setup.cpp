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

#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
      ESP32_WS2812 ws2812Bus0 = ESP32_WS2812(1, 21, 0, TYPE_RGB);

      ClassWS2812 indicator0({0,
                              false,
                              INDICATOR_BLINK_PERIOD_ON,
                              INDICATOR_BLINK_PERIOD_OFF},
                             &ws2812Bus0);

      ClassWS2812 indicator1({1,
                              false,
                              INDICATOR_BLINK_PERIOD_ON,
                              INDICATOR_BLINK_PERIOD_OFF},
                             &ws2812Bus0);

      ClassWS2812 indicator2({2,
                              false,
                              INDICATOR_BLINK_PERIOD_ON,
                              INDICATOR_BLINK_PERIOD_OFF},
                             &ws2812Bus0);
#endif

      void update()
      {
        indicator0.update();

#if defined(IOT_NODE_BOARD_ROOM_SENSOR) || defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO)
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
#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
        ws2812Bus0.begin();
#endif

        indicator0.init();

#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
        indicator1.init();
        indicator2.init();
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
            3,
            NULL,
            CONFIG_ARDUINO_RUNNING_CORE);
#endif
      }
    }

  } // section namespace
} // project namespace

#endif
