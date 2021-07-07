#pragma once

#ifdef IOT_NODE_ESP_NOW_GW

#include <Arduino.h>

#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

#include "../../utils/log.h"
#include "./main.h"

namespace IotNode {
namespace Events {

namespace EspNowGw {
  void setup();
}

} // section namespace
} // project namespace

#endif
