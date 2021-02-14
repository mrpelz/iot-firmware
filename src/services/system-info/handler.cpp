#include "./handler.h"

void systemInfoHandler(
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
) {
  debug("system-info-service", "got request");

  std::vector<uint8_t> response;

  #ifdef ARDUINO_ARCH_ESP8266
    auto chipId = ESP.getChipId();
    response.insert(response.end(), &chipId, &chipId + sizeof(chipId));

    auto flashId = ESP.getFlashChipId();
    response.insert(response.end(), &flashId, &flashId + sizeof(flashId));
  #endif

  auto macAddress = new uint8_t[6];
  WiFi.macAddress(macAddress);
  response.insert(response.end(), macAddress, macAddress + 6);
  delete macAddress;

  #ifndef IOT_NODE_LINK_ETH
    auto bssid = WiFi.BSSID();
    response.insert(response.end(), bssid, bssid + 6);

    auto channel = WiFi.channel();
    response.insert(response.end(), &channel, &channel + sizeof(channel));

    auto rssi = WiFi.RSSI();
    response.insert(response.end(), &rssi, &rssi + sizeof(rssi));
  #endif

  debug("system-info-service", "sending response");

  respond(response);
}
