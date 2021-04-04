#ifndef _UTILS_LOGGING
#define _UTILS_LOGGING

#include <Arduino.h>

#ifdef IOT_NODE_ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef IOT_NODE_ESP32
  #include <WiFi.h>
#endif

#ifndef IOT_NODE_LOG_DELAY
  #define IOT_NODE_LOG_DELAY 100
#endif

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
