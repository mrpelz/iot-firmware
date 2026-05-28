#include "./main.h"

namespace IotNode::Services::Hello
{
  auto delimiter = String(",");

  void addLineToRespone(Utils::UDP::Payload *response, const ::std::string line)
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
    Utils::Log::debug("hello-service: got request");
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
    addLineToRespone(&response, fmt::format("{:X}", ESP.getChipId()));
#endif
#ifdef IOT_NODE_ESP32
    addLineToRespone(&response, fmt::format("{:X}", ESP.getEfuseMac()));
#endif
    addDelimiter(&response);

#ifdef IOT_NODE_ESP8266
    addLineToRespone(&response, fmt::format("{:X}", ESP.getFlashChipId()));
#endif
    addDelimiter(&response);

#ifdef IOT_NODE_LINK_ETH
    addLineToRespone(&response, ETH.macAddress().c_str());
#endif
    addDelimiter(&response);

    addLineToRespone(&response, WiFi.macAddress().c_str());
    addDelimiter(&response);

#ifdef IOT_NODE_LINK_WIFI
    addLineToRespone(&response, Utils::Link::printMacAddress(WiFi.BSSID()));
#endif
    addDelimiter(&response);

#ifdef IOT_NODE_LINK_WIFI
    addLineToRespone(&response, ::std::to_string(WiFi.channel()));
#endif
    addDelimiter(&response);

#ifdef IOT_NODE_LINK_WIFI
    addLineToRespone(&response, ::std::to_string(WiFi.RSSI()));
#endif
    addDelimiter(&response);

#if defined(IOT_NODE_LINK_WIFI) && defined(IOT_NODE_ESP8266)
    addLineToRespone(&response, ::std::to_string(WiFi.getPhyMode()));
#endif
    addDelimiter(&response);

#ifdef IOT_NODE_LINK_WIFI
    addLineToRespone(&response, WiFi.SSID().c_str());
#endif
    addDelimiter(&response);

#ifdef IOT_NODE_ESP32
    addLineToRespone(&response, ::std::to_string(temperatureRead()));
#endif
    addDelimiter(&response);

    addLineToRespone(&response, "BYE");

#ifdef IOT_NODE_LOGGING
    Utils::Log::debug("hello-service: sending response");
#endif

    respond(response);
  }
}
