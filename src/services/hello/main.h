#ifndef _HELLO_MAIN
#define _HELLO_MAIN

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

namespace Hello {
  void addLineToRespone(UDP::Payload response, String line);

  void handler(UDP::Payload *request, UDP::RespondCallback respond);
}

} // project namespace

#endif
