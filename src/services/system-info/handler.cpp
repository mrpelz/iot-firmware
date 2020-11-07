#include "./handler.h"

void systemInfoHandler(
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
) {
  debug("event", "udp-service.systemInfo");

  std::vector<uint8_t> response;

  auto chipId = ESP.getChipId();
  response.insert(response.end(), &chipId, &chipId + sizeof(chipId));

  auto flashId = ESP.getChipId();
  response.insert(response.end(), &flashId, &flashId + sizeof(flashId));

  auto bssid = WiFi.BSSID();
  response.insert(response.end(), bssid, bssid + 6);

  auto channel = WiFi.channel();
  response.insert(response.end(), &channel, &channel + sizeof(channel));

  auto rssi = WiFi.RSSI();
  response.insert(response.end(), &rssi, &rssi + sizeof(rssi));

  respond(response);
}
