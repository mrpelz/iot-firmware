#include "./main.h"

namespace IotNode {
namespace Services {

namespace SystemInfo {
  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer) {
    Utils::Log::debug("system-info-service", "got request");

    Utils::UDP::Payload response;

    #ifdef IOT_NODE_ESP8266
      auto chipId = ESP.getChipId();
      response.insert(response.end(), &chipId, &chipId + sizeof(chipId));

      auto flashId = ESP.getFlashChipId();
      response.insert(response.end(), &flashId, &flashId + sizeof(flashId));
    #endif

    auto macAddress = new uint8_t[6];
    WiFi.macAddress(macAddress);
    response.insert(response.end(), macAddress, macAddress + 6);
    delete macAddress;

    #ifdef IOT_NODE_LINK_WIFI
      auto bssid = WiFi.BSSID();
      response.insert(response.end(), bssid, bssid + 6);

      auto channel = WiFi.channel();
      response.insert(response.end(), &channel, &channel + sizeof(channel));

      auto rssi = WiFi.RSSI();
      response.insert(response.end(), &rssi, &rssi + sizeof(rssi));
    #endif

    Utils::Log::debug("system-info-service", "sending response");

    respond(response);
  }
}

} // section namespace
} // project namespace
