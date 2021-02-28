#ifndef _SYSTEM_INFO_MAIN
#define _SYSTEM_INFO_MAIN

#include <Arduino.h>

#include "../../utils/log.h"
#include "../../utils/udp/main.h"

#ifdef ARDUINO_ARCH_ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>
#endif

#define TOSTRING(x) #x
#define STR(x) TOSTRING(x)

namespace IotNode {

namespace SystemInfo {
  void handler(UDP::Payload *request, UDP::RespondCallback respond);
}

} // project namespace

#endif
