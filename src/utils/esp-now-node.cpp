#include "./esp-now-node.h"

#ifdef IOT_NODE_ESP_NOW_NODE

namespace IotNode {
namespace Utils {

namespace EspNowNode {
  bool sent = false;

  WORKING_MODE workingMode = WORKING_MODE::WAKE;

  uint8_t gw_mac[] = IOT_NODE_ESP_NOW_GW_MAC;

  auto keepalive = Keepalive::Class(
    5000,
    []() {
      sleep();
    }
  );

  void sleep() {
    if (workingMode == WORKING_MODE::SLEEP) {
      ESP.deepSleep(0);
    }
  }

  void onDataSent(uint8_t *mac_addr, uint8_t status) {
    sleep();
  }

  void setup() {
    pinMode(ESP_NOW_BOOT_PIN, INPUT_PULLUP);
    workingMode = digitalRead(ESP_NOW_BOOT_PIN) ? WORKING_MODE::SLEEP : WORKING_MODE::WAKE;
    pinMode(ESP_NOW_BOOT_PIN, INPUT);

    if (workingMode == WORKING_MODE::WAKE) return;

    WiFi.persistent(false);

    WiFi.setAutoConnect(false);
    WiFi.setAutoReconnect(false);

    WiFi.mode(WIFI_STA);
    #ifdef IOT_NODE_ESP8266
      wifi_set_channel(IOT_NODE_ESP_NOW_WIFI_CHANNEL);
    #endif
    #ifdef IOT_NODE_ESP32
      esp_wifi_set_channel(IOT_NODE_ESP_NOW_WIFI_CHANNEL, WIFI_SECOND_CHAN_NONE);
    #endif

    if (esp_now_init() != 0) {
      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("error", "esp-now setup failed");
      #endif

      return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_add_peer(gw_mac, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
    esp_now_register_send_cb(onDataSent);
  }

  void send(std::vector<uint8_t> payload) {
    esp_now_send(NULL, payload.data(), payload.size());
  }

  void update() {
    if (workingMode == WORKING_MODE::SLEEP && !sent) {
      sent = true;
      keepalive.start();

      send({ 0xfa, 0xce, 0xb0, 0x0c });
    }

    keepalive.update();
  }
}

} // section namespace
} // project namespace

#endif
