#include "./main.h"

namespace IotNode {

namespace SystemInfo {
  void handler(UDP::Payload *request, UDP::RespondCallback respond) {
    Log::debug("system-info-service", "got request");

    UDP::Payload response;

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

    Log::debug("system-info-service", "sending response");

    respond(response);
  }
}

} // project namespace
