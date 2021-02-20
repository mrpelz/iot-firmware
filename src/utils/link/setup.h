#ifndef _PERSISTENT_LINK_SETUP
#define _PERSISTENT_LINK_SETUP

#define TOSTRING(x) #x
#define STR(x) TOSTRING(x)

#include <Arduino.h>

#include "./main.h"
#include "../udp/main.h"
#include "./utils/log.h"

#ifdef IOT_NODE_LINK_ETH
  #include "./eth.h"
#else
  #include "./wifi.h"
#endif

namespace IotNode {

namespace Link {
  static const Timings timings = {
    .runDebugEvery = 60000,
    .tryReconnectAfter = 2000,
    .restartAfter = 10000
  };

  #ifndef IOT_NODE_LINK_ETH
    #if defined(IOT_NODE_WIFI_SSID) && defined(IOT_NODE_WIFI_PSK)
      static const Credentials credentials = {
        .ssid = STR(IOT_NODE_WIFI_SSID),
        .password = STR(IOT_NODE_WIFI_PSK),

        #ifdef IOT_NODE_ADVANCED_WIFI_CONFIG
          #if defined(IOT_NODE_AP_ALGORE)
            .bssid = { 0x7a, 0x8a, 0x20, 0x84, 0x69, 0x8c },
            .channel = 11,
          #elif defined(IOT_NODE_AP_ELZAR)
            .bssid = { 0x7a, 0x8a, 0x20, 0x2d, 0x4b, 0x91 },
            .channel = 1,
          #elif defined(IOT_NODE_AP_RICHARDNIXON)
            .bssid = { 0x7a, 0x8a, 0x20, 0x81, 0xd3, 0x2b },
            .channel = 6,
          #elif defined(IOT_NODE_AP_SPIROAGNEW)
            .bssid = { 0xf2, 0x9f, 0xc2, 0xc9, 0xb6, 0x18 },
            .channel = 6,
          #endif
        #endif
      };
    #endif

    #ifdef ARDUINO_ARCH_ESP8266
      #include <ESP8266WiFi.h>

      static const PhyMode_t phyMode = WIFI_PHY_MODE_11N;

      static const OutputPower_t outputPower = 6;
    #endif

    #ifdef ARDUINO_ARCH_ESP32
      #include <WiFi.h>

      static const PhyMode_t phyMode = WIFI_PHY_RATE_54M;

      static const OutputPower_t outputPower = WIFI_POWER_7dBm;
    #endif
  #endif

  #ifndef IOT_NODE_DHCP
    static const InterfaceConfig interfaceConfig = {
      .ip = IPAddress(10, 97, 4, 254),
      .gateway = IPAddress(10, 97, 4, 1),
      .netmask = IPAddress(255, 255, 255, 0),
    };
  #endif

  #ifdef IOT_NODE_LINK_ETH
    #ifdef ARDUINO_ARCH_ESP32
      static const Config config = {
        #ifndef IOT_NODE_DHCP
          .interfaceConfig = interfaceConfig,
        #endif

        .timings = timings
      };
    #endif
  #else
    static const Config config = {
      .phyMode = phyMode,
      .outputPower = outputPower,

      #ifndef IOT_NODE_DHCP
        .interfaceConfig = interfaceConfig,
      #endif

      #ifndef IOT_NODE_LINK_ETH
        .credentials = credentials,
      #endif

      .timings = timings
    };
  #endif

  void task(void * parameter);

  Class *setup(UDP::Class *udp);
}

} // project namespace

#endif
