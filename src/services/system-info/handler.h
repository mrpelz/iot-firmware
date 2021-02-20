#ifndef _SYSTEM_INFO_HANDLER
#define _SYSTEM_INFO_HANDLER

#include <Arduino.h>

#ifdef ARDUINO_ARCH_ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>
#endif

#include "../../utils/udp.h"
#include "../../utils/log.h"

namespace IotNode {

void systemInfoHandler(
    std::vector<uint8_t> *request,
    RespondCallback respond
);

} // project namespace

#endif
