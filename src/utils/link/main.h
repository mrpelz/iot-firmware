#pragma once

#include <Arduino.h>

#include <pre.h>
#include <post.h>
#include <functional>
#include <FmtLib.h>

#include <pre.h>

#ifdef IOT_NODE_ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef IOT_NODE_ESP32
#include <WiFi.h>
#endif

#include "../log.h"

#define LINK_DELAY 1000

namespace IotNode
{
  namespace Utils
  {

    namespace Link
    {
#ifdef IOT_NODE_ESP8266
      typedef WiFiEventHandler EventHandler_t;
      typedef WiFiDisconnectReason DisconnectReason_t;
#endif
#ifdef IOT_NODE_ESP32
      typedef wifi_event_id_t EventHandler_t;
      typedef uint8_t DisconnectReason_t;
#endif

      ::std::string printMacAddress(uint8_t input[6]);

#ifdef IOT_NODE_IP_STATIC
      struct InterfaceConfig
      {
        IPAddress ip;
        IPAddress gateway;
        IPAddress netmask;
      };
#endif

      struct Timings
      {
        unsigned long runDebugEvery;
        unsigned long tryReconnectAfter;
        unsigned long restartAfter;
      };

      struct EventListeners
      {
        EventHandler_t onConnected;
        EventHandler_t onDisconnected;
        EventHandler_t onGotIP;
        EventHandler_t onDHCPTimeout;
      };

      struct Callbacks
      {
        ::std::function<void()> beforeRestart = []() {};
        ::std::function<void()> reconnect = []() {};

        ::std::function<void()> connected = []() {};
        ::std::function<void()> dhcpTimeout = []() {};
        ::std::function<void()> disconnected = []() {};
        ::std::function<void(IPAddress ip)> gotIP = [](auto ip) {};
      };
    }

  } // section namespace
} // project namespace
