#ifndef _SYSTEM_INFO_HANDLER
#define _SYSTEM_INFO_HANDLER

#define TOSTRING(x) #x
#define STR(x) TOSTRING(x)

#include <Arduino.h>

#ifdef ARDUINO_ARCH_ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>
#endif

#include "../../logging.h"

void systemInfoHandler(
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
);

#endif
