#include "./main.h"

namespace IotNode {
namespace Services {

namespace Hello {
  auto delimiter = String(",");

  void addLineToRespone(Utils::UDP::Payload *response, String line) {
    response->insert(response->end(), line.begin(), line.end());
    response->insert(response->end(), delimiter.begin(), delimiter.end());
  }

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer) {
    #ifdef IOT_NODE_LOGGING
      Utils::Log::debug("hello-service", "got request");
    #endif

    Utils::UDP::Payload response;

    addLineToRespone(&response, "HELLO");
    addLineToRespone(&response, IOT_NODE_NAME);

    #ifdef IOT_NODE_BOARD_NAME
      addLineToRespone(&response, IOT_NODE_BOARD_NAME);
    #endif

    #ifdef IOT_NODE_HARDWARE_NAME
      addLineToRespone(&response, IOT_NODE_HARDWARE_NAME);
    #endif

    addLineToRespone(&response, IOT_NODE_BUILD_GIT_REV);
    addLineToRespone(&response, IOT_NODE_PIO_ENV);
    addLineToRespone(&response, IOT_NODE_PIO_PLATFORM);
    addLineToRespone(&response, IOT_NODE_PIO_FRAMEWORK);

    #ifdef IOT_NODE_ESP8266
      addLineToRespone(&response, String(ESP.getChipId(), HEX));
      addLineToRespone(&response, String(ESP.getFlashChipId(), HEX));
    #endif

    addLineToRespone(&response, WiFi.macAddress());

    #ifdef IOT_NODE_LINK_WIFI
      addLineToRespone(&response, Utils::Link::printMacAddress(WiFi.BSSID()));
      addLineToRespone(&response, String(WiFi.channel()));
      addLineToRespone(&response, String(WiFi.RSSI()));

      #ifdef IOT_NODE_ESP8266
        addLineToRespone(&response, String(WiFi.getPhyMode()));
      #endif

      addLineToRespone(&response, WiFi.SSID());
    #endif

    addLineToRespone(&response, "BYE");

    #ifdef IOT_NODE_LOGGING
      Utils::Log::debug("hello-service", "sending response");
    #endif

    respond(response);
  }
}

} // section namespace
} // project namespace
