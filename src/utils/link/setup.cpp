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
                       IotNode::Utils::UDP::instance.begin();

#ifdef IOT_NODE_ESP32
                       MDNS.begin(IOT_NODE_NAME);

                       ArduinoOTA.setMdnsEnabled(true);
                       ArduinoOTA.begin();
#endif

#ifdef IOT_NODE_ESP8266
                       MDNS.begin(IOT_NODE_NAME, ip);

                       ArduinoOTA.begin(true);
#endif

#ifndef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
#ifdef IOT_NODE_INDICATORS
                       Indicator::indicator0.setOn(false);
#endif
#endif
                     });

        link.onDisconnected([]()
                            {
                              IotNode::Utils::UDP::instance.close();

#ifdef IOT_NODE_ESP32
                              ArduinoOTA.end();
#endif

#ifndef IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO
#ifdef IOT_NODE_INDICATORS
                              Indicator::indicator0.blink();
#endif
#endif
                            });

        link.connect();

#ifdef IOT_NODE_ESP32
        xTaskCreatePinnedToCore(
            task,
            "link_maintenance",
            4096,
            NULL,
            tskIDLE_PRIORITY,
            NULL,
            CONFIG_ARDUINO_RUNNING_CORE);
#endif
      }
    }

  } // section namespace
} // project namespace
