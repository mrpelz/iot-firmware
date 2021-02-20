#ifndef _HELLO_HANDLER
#define _HELLO_HANDLER

#define TOSTRING(x) #x
#define STR(x) TOSTRING(x)

#include <Arduino.h>
#include "../../utils/udp/main.h"
#include "../../utils/log.h"
#include "../../utils/link/main.h"

#ifdef ARDUINO_ARCH_ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>
#endif

namespace IotNode {

void addLineToRespone(UDP::Payload response, String line);

void helloHandler(
    UDP::Payload *request,
    UDP::RespondCallback respond
);

} // project namespace

#endif
