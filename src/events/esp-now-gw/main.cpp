#include "./main.h"

#ifdef IOT_NODE_ESP_NOW_GW

namespace IotNode::Events::EspNowGw
{
  void event(::std::vector<uint8_t> payload)
  {
    Utils::UDP::instance.event(ids::espNowGw, 0, payload);
  }

  void onDataReceived(const uint8_t *mac, const uint8_t *incomingData, int length)
  {
#ifdef IOT_NODE_LOGGING
    Utils::Log::debug("event: esp-now-message");
    Utils::Log::debug("esp-now-message.mac", Utils::Link::printMacAddress(WiFi.BSSID()));
#endif

    ::std::vector<uint8_t> payload = {};

    payload.insert(
        payload.end(),
        mac,
        mac + 6);

    payload.insert(
        payload.end(),
        incomingData,
        incomingData + length);

    event(payload);
  }
}

#endif
