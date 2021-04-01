#ifndef _SERVICES_SYSTEM_INFO_MAIN
#define _SERVICES_SYSTEM_INFO_MAIN

#include <Arduino.h>

#include "../../utils/log.h"
#include "../../utils/udp/main.h"

#ifdef IOT_NODE_ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef IOT_NODE_ESP32
  #include <WiFi.h>
#endif

namespace IotNode {
namespace Services {

namespace SystemInfo {
  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond);
}

} // section namespace
} // project namespace

#endif
