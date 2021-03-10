#ifndef _SERVICES_HELLO_MAIN
#define _SERVICES_HELLO_MAIN

#include <Arduino.h>

#include "../../utils/link/main.h"
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
namespace Services {

namespace Hello {
  void addLineToRespone(Utils::UDP::Payload response, String line);

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond);
}

} // section namespace
} // project namespace

#endif
