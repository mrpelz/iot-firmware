#ifndef _SYSTEM_INFO_HANDLER
#define _SYSTEM_INFO_HANDLER

#define TOSTRING(x) #x
#define STR(x) TOSTRING(x)

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "../../logging.h"

void systemInfoHandler(
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
);

#endif
