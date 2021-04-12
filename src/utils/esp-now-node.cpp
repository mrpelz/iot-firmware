#include "./esp-now-node.h"

#ifdef IOT_NODE_ESP_NOW_NODE

namespace IotNode {
namespace Utils {

namespace EspNowNode {
  uint8_t gw_mac[] = IOT_NODE_ESP_NOW_GW_MAC;

  bool setup() {
    WiFi.persistent(false);

    WiFi.setAutoConnect(false);
    WiFi.setAutoReconnect(false);

    WiFi.mode(WIFI_STA);
    #ifdef IOT_NODE_ESP8266
      wifi_set_channel(1);
    #endif
    #ifdef IOT_NODE_ESP32
      esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
    #endif

    if (esp_now_init() != 0) {
      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("error", "esp-now setup failed");
      #endif

      return false;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_add_peer(gw_mac, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

    pinMode(ESP_NOW_BOOT_PIN, INPUT_PULLUP);
    auto result = !!digitalRead(ESP_NOW_BOOT_PIN);
    pinMode(ESP_NOW_BOOT_PIN, INPUT);
    
    return result;
  }

  void send(std::vector<uint8_t> payload) {
    esp_now_send(NULL, payload.data(), payload.size());
  }
}

} // section namespace
} // project namespace

#endif
