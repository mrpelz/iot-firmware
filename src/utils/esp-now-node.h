#ifndef _UTILS_ESP_NOW_NODE
#define _UTILS_ESP_NOW_NODE

#ifdef IOT_NODE_ESP_NOW_NODE

#include <Arduino.h>

#ifdef IOT_NODE_ESP8266
  #include <ESP8266WiFi.h>
  #include <espnow.h>
#endif
#ifdef IOT_NODE_ESP32
  #include <WiFi.h>
  #include <esp_now.h>
#endif

#include "./utils/log.h"

namespace IotNode {
namespace Utils {

namespace EspNowNode {
  void setup();

  void send(std::vector<uint8_t> payload);
}

} // section namespace
} // project namespace

#endif

#endif
