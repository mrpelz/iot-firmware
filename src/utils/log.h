#ifndef _UTILS_LOGGING
#define _UTILS_LOGGING

#include <Arduino.h>

#ifdef ARDUINO_ARCH_ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>
#endif

#define TOSTRING(x) #x
#define STR(x) TOSTRING(x)

#define LOG_DELAY 100

namespace IotNode {
namespace Utils {

namespace Log {
  typedef std::function<void (String key, String value)> Callback;

  void setup();

  void debug(String key, String value);

  void info();
}

} // section namespace
} // project namespace

#endif
