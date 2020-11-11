#ifndef _HELLO_HANDLER
#define _HELLO_HANDLER

#define TOSTRING(x) #x
#define STR(x) TOSTRING(x)

#include <Arduino.h>
#include "../../logging.h"
#include "../../persistent-link/tools.h"

#ifdef ARDUINO_ARCH_ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>
#endif

void addLineToRespone(std::vector<uint8_t> response, String line);

void helloHandler(
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
);

#endif
