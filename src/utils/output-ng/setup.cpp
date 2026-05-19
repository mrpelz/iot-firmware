#include "./setup.h"

#ifdef IOT_NODE_INDICATORS

namespace IotNode
{
  namespace Utils
  {

    namespace OutputNg
    {
      Dimmable indicator0(1, true);
      DimmableRGB indicator1(2, 3, 4, false);

#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
      ESP32_WS2812 ws2812Bus0 = ESP32_WS2812(1, 21, 0, TYPE_RGB);
      DimmableRGBWS2812 indicatorRGB(0, &ws2812Bus0);
#endif

      void update()
      {
        indicator0.update();
        indicator1.update();

#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
        indicatorRGB.update();
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
        indicator0.init();
        indicator0.blink();

        indicator1.init();
        indicator1.blinkRGB();

#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
        ws2812Bus0.begin();
        indicatorRGB.init();
        indicatorRGB.blinkRGBInclusive();
#endif

#ifdef IOT_NODE_ESP32
        xTaskCreatePinnedToCore(
            task,
            "output_ng_maintenance",
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
