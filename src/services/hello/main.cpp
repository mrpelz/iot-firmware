#include "./main.h"

namespace IotNode
{
  namespace Services
  {

    namespace Hello
    {
      auto delimiter = String(",");

      void addLineToRespone(Utils::UDP::Payload *response, String line)
      {
        response->insert(response->end(), line.begin(), line.end());
      }

      void addDelimiter(Utils::UDP::Payload *response)
      {
        response->insert(response->end(), delimiter.begin(), delimiter.end());
      }

      void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer)
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("hello-service", "got request");
#endif

        Utils::UDP::Payload response;

        addLineToRespone(&response, "HELLO");
        addDelimiter(&response);

        addLineToRespone(&response, IOT_NODE_NAME);
        addDelimiter(&response);

#ifdef IOT_NODE_BOARD_NAME
        addLineToRespone(&response, IOT_NODE_BOARD_NAME);
#endif
        addDelimiter(&response);

#ifdef IOT_NODE_HARDWARE_NAME
        addLineToRespone(&response, IOT_NODE_HARDWARE_NAME);
#endif
        addDelimiter(&response);

        addLineToRespone(&response, IOT_NODE_BUILD_GIT_REV);
        addDelimiter(&response);

        addLineToRespone(&response, IOT_NODE_PIO_ENV);
        addDelimiter(&response);

        addLineToRespone(&response, IOT_NODE_PIO_PLATFORM);
        addDelimiter(&response);

        addLineToRespone(&response, IOT_NODE_PIO_FRAMEWORK);
        addDelimiter(&response);

#ifdef IOT_NODE_ESP8266
        addLineToRespone(&response, String(ESP.getChipId(), HEX));
#endif
#ifdef IOT_NODE_ESP32
        addLineToRespone(&response, String(ESP.getEfuseMac(), HEX));
#endif
        addDelimiter(&response);

#ifdef IOT_NODE_ESP8266
        addLineToRespone(&response, String(ESP.getFlashChipId(), HEX));
#endif
        addDelimiter(&response);

#ifdef IOT_NODE_LINK_ETH
        addLineToRespone(&response, ETH.macAddress());
#endif
        addDelimiter(&response);

        addLineToRespone(&response, WiFi.macAddress());
        addDelimiter(&response);

#ifdef IOT_NODE_LINK_WIFI
        addLineToRespone(&response, Utils::Link::printMacAddress(WiFi.BSSID()));
#endif
        addDelimiter(&response);

#ifdef IOT_NODE_LINK_WIFI
        addLineToRespone(&response, String(WiFi.channel()));
#endif
        addDelimiter(&response);

#ifdef IOT_NODE_LINK_WIFI
        addLineToRespone(&response, String(WiFi.RSSI()));
#endif
        addDelimiter(&response);

#if defined(IOT_NODE_LINK_WIFI) && defined(IOT_NODE_ESP8266)
        addLineToRespone(&response, String(WiFi.getPhyMode()));
#endif
        addDelimiter(&response);

#ifdef IOT_NODE_LINK_WIFI
        addLineToRespone(&response, WiFi.SSID());
#endif
        addDelimiter(&response);

#ifdef IOT_NODE_ESP32
        addLineToRespone(&response, String(temperatureRead()));
#endif
        addDelimiter(&response);

        addLineToRespone(&response, "BYE");

#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("hello-service", "sending response");
#endif

        respond(response);
      }
    }

  } // section namespace
} // project namespace
