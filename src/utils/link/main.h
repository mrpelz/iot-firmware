#ifndef _LINK_MAIN
#define _LINK_MAIN

#include <Arduino.h>

#ifdef ARDUINO_ARCH_ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>
#endif

#include "../log.h"

#define LINK_DELAY 1000

namespace IotNode {

namespace Link {
  #ifdef ARDUINO_ARCH_ESP8266
    typedef WiFiEventHandler EventHandler_t;
    typedef WiFiDisconnectReason DisconnectReason_t;
  #endif
  #ifdef ARDUINO_ARCH_ESP32
    typedef wifi_event_id_t EventHandler_t;
    typedef uint8_t DisconnectReason_t;
  #endif

  String printMacAddress(uint8_t input[6]);

  #ifndef IOT_NODE_DHCP
    struct InterfaceConfig {
      IPAddress ip;
      IPAddress gateway;
      IPAddress netmask;
    };
  #endif

  struct Timings {
    unsigned long runDebugEvery;
    unsigned long tryReconnectAfter;
    unsigned long restartAfter;
  };

  struct EventListeners {
    EventHandler_t onConnected;
    EventHandler_t onDisconnected;
    EventHandler_t onGotIP;
    EventHandler_t onDHCPTimeout;
  };

  struct Callbacks {
    Log::Callback debug = [](String key, String value) {};

    std::function<void ()> beforeRestart = []() {};
    std::function<void ()> reconnect = []() {};

    std::function<void ()> connected = []() {};
    std::function<void ()> dhcpTimeout = []() {};
    std::function<void ()> disconnected = []() {};
    std::function<void ()> gotIP = []() {};
  };
}

} // project namespace

#endif
