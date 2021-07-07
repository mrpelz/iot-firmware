#pragma once

#ifdef IOT_NODE_LOGGING

#include <Arduino.h>
#include <functional>

#ifdef IOT_NODE_ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef IOT_NODE_ESP32
  #include <WiFi.h>
#endif

namespace IotNode {
namespace Utils {

namespace Log {
  typedef std::function<void (String key, String value)> LogCallback;

  void setup();

  void debug(String key, String value);

  void info();
}

} // section namespace
} // project namespace

#endif
