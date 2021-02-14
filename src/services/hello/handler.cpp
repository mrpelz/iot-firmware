#include "./handler.h"

auto delimiter = String(",");

void addLineToRespone(std::vector<uint8_t> *response, String line) {
  response->insert(response->end(), line.begin(), line.end());
  response->insert(response->end(), delimiter.begin(), delimiter.end());
}

void helloHandler(
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
) {
  debug("hello-service", "got request");

  std::vector<uint8_t> response;

  addLineToRespone(&response, "HELLO");
  addLineToRespone(&response, STR(IOT_NODE_BUILD_GIT_REV));
  addLineToRespone(&response, STR(IOT_NODE_PIO_ENV));
  addLineToRespone(&response, STR(IOT_NODE_PIO_PLATFORM));
  addLineToRespone(&response, STR(IOT_NODE_PIO_FRAMEWORK));

  #ifdef ARDUINO_ARCH_ESP8266
    addLineToRespone(&response, String(ESP.getChipId(), HEX));
    addLineToRespone(&response, String(ESP.getFlashChipId(), HEX));
  #endif

  addLineToRespone(&response, WiFi.macAddress());

  #ifndef IOT_NODE_LINK_ETH
    addLineToRespone(&response, printMacAddress(WiFi.BSSID()));
    addLineToRespone(&response, String(WiFi.channel()));
    addLineToRespone(&response, String(WiFi.RSSI()));

    #ifdef ARDUINO_ARCH_ESP8266
      addLineToRespone(&response, String(WiFi.getPhyMode()));
    #endif

    addLineToRespone(&response, WiFi.SSID());
  #endif

  addLineToRespone(&response, "BYE");

  debug("hello-service", "sending response");

  respond(response);
}
