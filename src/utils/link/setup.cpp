#include "./setup.h"

namespace IotNode
{
  namespace Utils
  {

    namespace Link
    {
      Class link(config);

      void update()
      {
        link.update();
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
        link.configDebug();

        link.onGotIP([](IPAddress ip)
                     {
                       UDP::instance.begin();

#ifdef IOT_NODE_ESP32
                       MDNS.begin(IOT_NODE_NAME);

                       ArduinoOTA.setMdnsEnabled(true);
                       ArduinoOTA.begin();
#endif

#ifdef IOT_NODE_ESP8266
                       MDNS.begin(IOT_NODE_NAME, ip);

                       ArduinoOTA.begin(true);
#endif

#ifdef IOT_NODE_INDICATORS
                       Indicator::indicator0.setOn(false);

#ifdef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
                       Indicator::indicator1.blink(3);
#endif

#endif
                     });

        link.onDisconnected([]()
                            {
                              UDP::instance.close();

#ifdef IOT_NODE_ESP32
                              ArduinoOTA.end();
#endif

#ifdef IOT_NODE_INDICATORS
                              Indicator::indicator0.blink();
#endif
                            });

        link.connect();

#ifdef IOT_NODE_ESP32
        xTaskCreatePinnedToCore(
            task,
            "link_maintenance",
            4096,
            NULL,
            2,
            NULL,
            CONFIG_ARDUINO_RUNNING_CORE);
#endif
      }
    }

  } // section namespace
} // project namespace
