#pragma once

#ifdef IOT_NODE_LOGGING

#include <Arduino.h>
#include <functional>

#include <pre.h>
#include <post.h>

#include <FmtLib.h>

#ifdef IOT_NODE_ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef IOT_NODE_ESP32
#include <WiFi.h>
#endif

namespace IotNode
{
  namespace Utils
  {

    namespace Log
    {
      typedef ::std::function<void(String key, String value)> LogCallback;

      void setup();

      void debug(::std::string key, ::std::string value);
      void debug(::std::string value);

      void info();
    }

  } // section namespace
} // project namespace

#endif
