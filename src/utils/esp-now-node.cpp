#include "./esp-now-node.h"

#ifdef IOT_NODE_ESP_NOW_NODE

namespace IotNode
{
  namespace Utils
  {

    namespace EspNowNode
    {
      bool espNowInitialized = false;

      WORKING_MODE workingMode = WORKING_MODE::WAKE;

      uint8_t gw_mac[] = IOT_NODE_ESP_NOW_GW_MAC;

      auto keepalive = Keepalive::Class(
          5000,
          []()
          {
            sleep();
          });

      void getWorkingMode()
      {
#ifdef IOT_NODE_BOARD_ESP_NOW_TEST_BUTTON
        pinMode(4, INPUT_PULLUP);
        pinMode(14, INPUT_PULLUP);
        // only enter wake mode if both buttons are pressed down (pulled to GND)
        workingMode = digitalRead(4) || digitalRead(14) ? WORKING_MODE::SLEEP : WORKING_MODE::WAKE;
        pinMode(4, INPUT);
        pinMode(14, INPUT);
#elif defined(IOT_NODE_BOARD_ESP_NOW_TEST_WINDOW_SENSOR)
        pinMode(16, INPUT_PULLUP);
        workingMode = digitalRead(16) ? WORKING_MODE::SLEEP : WORKING_MODE::WAKE;
        pinMode(16, INPUT);
#endif
      }

      void sleep()
      {
        if (workingMode == WORKING_MODE::SLEEP)
        {
          ESP.deepSleep(0);
        }
      }

      void onDataSent(uint8_t *mac_addr, uint8_t status)
      {
        keepalive.tick();
      }

      void setup()
      {
        WiFi.persistent(false);

        WiFi.setAutoConnect(false);
        WiFi.setAutoReconnect(false);

        WiFi.mode(WIFI_STA);
#ifdef IOT_NODE_ESP8266
        wifi_set_channel(IOT_NODE_ESP_NOW_WIFI_CHANNEL);
#endif
#ifdef IOT_NODE_ESP32
        esp_wifi_set_channel(IOT_NODE_ESP_NOW_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);
#endif

        if (esp_now_init() != 0)
        {
#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("error", "esp-now setup failed");
#endif

          sleep();
          return;
        }

        esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
        esp_now_add_peer(gw_mac, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
        esp_now_register_send_cb(onDataSent);

        espNowInitialized = true;
        keepalive.start();
      }

      void send(std::vector<uint8_t> payload)
      {
        if (workingMode == WORKING_MODE::WAKE || !espNowInitialized)
          return;

        keepalive.tick();
        esp_now_send(NULL, payload.data(), payload.size());
      }

      void update()
      {
        keepalive.update();
      }
    }

  } // section namespace
} // project namespace

#endif
