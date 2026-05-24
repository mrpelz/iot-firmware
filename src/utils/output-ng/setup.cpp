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
      Buzzer buzzer(5, true);

#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
      ESP32_WS2812 ws2812Bus0 = ESP32_WS2812(1, 21, 0, TYPE_RGB);
      DimmableRGBWS2812 indicatorRGB(0, &ws2812Bus0, false);
#endif

      void update()
      {
        indicator0.update();
        indicator1.update();
        buzzer.update();

#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
        indicatorRGB.update();
        ws2812Bus0.show();
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
        indicator1.blinkRGBInclusive();

        buzzer.init();

        // buzzer.beep(1);

        // buzzer.melody({OUTPUT_BUZZER_DEFAULT_FREQUENCY / 2,
        //                OUTPUT_BUZZER_DEFAULT_FREQUENCY,
        //                OUTPUT_BUZZER_DEFAULT_FREQUENCY * 2},
        //               1, 200, 0);

        buzzer.melody({OUTPUT_BUZZER_DEFAULT_FREQUENCY,
                       OUTPUT_BUZZER_DEFAULT_FREQUENCY * 2},
                      1, 200, 0);

        // buzzer.setSequence({.iterations = 1,
        //                     .sequence = {
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_C_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_C_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_D_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_E_FLAT_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_E_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_F_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_F_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_G_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_G_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_A_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_B_FLAT_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_B_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_B_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_B_FLAT_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_A_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_G_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_G_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_F_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_F_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_E_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_E_FLAT_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_D_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_C_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_C_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         // sweep
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_C_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_C_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_C_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_C_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_C_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_D_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_D_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_E_FLAT_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_E_FLAT_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_E_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_E_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_F_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_F_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_F_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_F_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_G_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_G_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_G_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_G_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_A_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_A_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_B_FLAT_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_B_FLAT_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_B_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_B_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_B_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_B_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_B_FLAT_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_B_FLAT_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_A_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_A_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_G_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_G_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_G_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_G_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_F_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_F_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_F_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_F_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_E_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_E_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_E_FLAT_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_E_FLAT_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_D_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_D_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_C_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_C_SHARP_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 875,
        //                                 .value = OUTPUT_BUZZER_C_FREQUENCY,
        //                             },
        //                             .holdTime = 875,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = OUTPUT_BUZZER_C_FREQUENCY,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                         {
        //                             .value = {
        //                                 .rampTime = 0,
        //                                 .value = 0,
        //                             },
        //                             .holdTime = 125,
        //                         },
        //                     }});

#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
        ws2812Bus0.begin();
        indicatorRGB.init();
        indicatorRGB.blinkRGBInclusive();
#endif

#ifdef IOT_NODE_ESP32
        xTaskCreatePinnedToCore(
            task,
            "output_ng_maintenance",
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
