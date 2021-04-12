#ifndef _UTILS_ESP_NOW_NODE
#define _UTILS_ESP_NOW_NODE

#ifdef IOT_NODE_ESP_NOW_NODE

#include <Arduino.h>

#ifdef IOT_NODE_ESP8266
  #include <ESP8266WiFi.h>
  #include <espnow.h>
#endif
#ifdef IOT_NODE_ESP32
  #include <esp_now.h>
  #include <esp_wifi.h>
  #include <WiFi.h>
#endif

#include "./utils/log.h"

#define ESP_NOW_BOOT_PIN 3

namespace IotNode {
namespace Utils {

namespace EspNowNode {
  bool setup();

  void send(std::vector<uint8_t> payload);
}

} // section namespace
} // project namespace

#endif

#endif
