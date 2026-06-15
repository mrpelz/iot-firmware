#include "./main.h"

#ifdef IOT_NODE_ESP_NOW_GW

namespace IotNode::Events::EspNowGw
{
  void event(::std::vector<unsigned char> payload)
  {
    Utils::UDP::instance.event(ids::espNowGw, 0, payload);
  }

  void onDataReceived(const esp_now_recv_info_t *esp_now_info, const unsigned char *incomingData, int length)
  {
#ifdef IOT_NODE_LOGGING
    Utils::Log::debug("event: esp-now-message");
    Utils::Log::debug("esp-now-message.mac", Utils::Link::printMacAddress(WiFi.BSSID()));
#endif

    ::std::vector<unsigned char> payload = {};

    payload.insert(
        payload.end(),
        esp_now_info->src_addr,
        esp_now_info->src_addr + 6);

    payload.insert(
        payload.end(),
        incomingData,
        incomingData + length);

    event(payload);
  }
}

#endif
