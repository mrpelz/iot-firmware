#ifndef _LOGGING
#define _LOGGING

#include <Arduino.h>
#include <ESP8266WiFi.h>

#define TOSTRING(x) #x
#define STR(x) TOSTRING(x)

#ifdef IOT_NODE_DEFER_INITIAL_LOGGING
  // 0: wifi not started yet, defer infoLog
  // 1: wifi started, run infoLog
  // 2: infoLog has already been executed, do nothing
  uint8_t infoLog = 0;
#endif

void debug(String key, String value);

void setupInfoLog();

#endif
