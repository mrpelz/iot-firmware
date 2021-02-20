#ifndef _HELLO_HANDLER
#define _HELLO_HANDLER

#define TOSTRING(x) #x
#define STR(x) TOSTRING(x)

#include <Arduino.h>
#include "../../utils/udp-messaging.h"
#include "../../utils/logging.h"
#include "../../utils/link/tools.h"

#ifdef ARDUINO_ARCH_ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>
#endif

void addLineToRespone(std::vector<uint8_t> response, String line);

void helloHandler(
    std::vector<uint8_t> *request,
    RespondCallback respond
);

#endif
