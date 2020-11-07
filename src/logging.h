#ifndef _LOGGING
#define _LOGGING

#include <Arduino.h>
#include <ESP8266WiFi.h>

#define TOSTRING(x) #x
#define STR(x) TOSTRING(x)

void debug(String key, String value);

void setupInfoLog();

#endif
