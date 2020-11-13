#ifndef _LOGGING
#define _LOGGING

#include <Arduino.h>

#ifdef ARDUINO_ARCH_ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>
#endif

#define TOSTRING(x) #x
#define STR(x) TOSTRING(x)

typedef std::function<void (const String &, const String &)> LoggingCallback;

void debug(String key, String value);

void setupInfoLog();

#endif
