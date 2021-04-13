#include "./setup.h"

#ifdef IOT_NODE_ESP_NOW_GW

namespace IotNode {
namespace Events {

namespace EspNowGw {
  void setup() {
    WiFi.persistent(false);

    WiFi.setAutoConnect(false);
    WiFi.setAutoReconnect(false);

    WiFi.mode(WIFI_STA);
    esp_wifi_set_channel(IOT_NODE_ESP_NOW_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);

    if (esp_now_init() != 0) {
      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("error", "esp-now setup failed");
      #endif

      return;
    }

    esp_now_register_recv_cb(onDataReceived);
  }
}

} // section namespace
} // project namespace

#endif
