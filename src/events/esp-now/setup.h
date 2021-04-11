#ifndef _EVENTS_ESP_NOW_SETUP
#define _EVENTS_ESP_NOW_SETUP

#ifdef IOT_NODE_ESP_NOW_GW

#include <Arduino.h>

#include <WiFi.h>
#include <esp_now.h>

#include "../../utils/log.h"
#include "./main.h"

namespace IotNode {
namespace Events {

namespace EspNow {
  void setup();
}

} // section namespace
} // project namespace

#endif

#endif
