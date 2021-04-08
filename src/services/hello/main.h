#ifndef _SERVICES_HELLO_MAIN
#define _SERVICES_HELLO_MAIN

#include <Arduino.h>

#include "../../utils/link/main.h"
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

namespace Hello {
  void addLineToRespone(Utils::UDP::Payload response, String line);

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer);
}

} // section namespace
} // project namespace

#endif
