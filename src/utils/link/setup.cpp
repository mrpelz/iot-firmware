#include "./setup.h"

namespace IotNode::Utils::Link
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
      vTaskDelay(pdMS_TO_TICKS(100));
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
                   MDNS.begin(IOT_NODE_PIO_ENV);

                   ArduinoOTA.setMdnsEnabled(true);
                   ArduinoOTA.begin();
#endif

#ifdef IOT_NODE_ESP8266
                   MDNS.begin(IOT_NODE_PIO_ENV, ip);

                   ArduinoOTA.begin(true);
#endif

#if !defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO) && !defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ETH)
#ifdef IOT_NODE_INDICATORS
                   Setup::Indicator::indicator0.setOn(false);
#endif
#endif
                 });

    link.onDisconnected([]()
                        {
                          IotNode::Utils::UDP::instance.close();

#ifdef IOT_NODE_ESP32
                          ArduinoOTA.end();
#endif

#if !defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ZERO) && !defined(IOT_NODE_BOARD_WAVESHARE_ESP32_S3_ETH)
#ifdef IOT_NODE_INDICATORS
#if defined(ENV_SCOPED_SETUP) && (defined(IOT_NODE_BOARD_TEST_DEVICE) || defined(IOT_NODE_BOARD_ROOM_SENSOR) || defined(IOT_NODE_BOARD_OBI_JACK) || defined(IOT_NODE_BOARD_H801) || defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V1) || defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V13) || defined(IOT_NODE_BOARD_SHELLYPLUS1))
                          Setup::Indicator::indicator0.blink();
#else
                          Indicator::indicator0.blink();
#endif
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
