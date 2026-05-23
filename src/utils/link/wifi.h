#pragma once

#ifdef IOT_NODE_LINK_WIFI

#include <Arduino.h>

#ifdef IOT_NODE_ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef IOT_NODE_ESP32
#include <WiFi.h>
#endif

#include "./main.h"

namespace IotNode
{
  namespace Utils
  {

    namespace Link
    {
#ifdef IOT_NODE_ESP8266
      typedef float OutputPower_t;
      typedef WiFiPhyMode_t PhyMode_t;
#endif
#ifdef IOT_NODE_ESP32
      typedef wifi_phy_rate_t PhyMode_t;
      typedef wifi_power_t OutputPower_t;
#endif

      struct Credentials
      {
        std::string ssid;
        std::string password;

#ifdef IOT_NODE_ADVANCED_WIFI_CONFIG
        uint8_t bssid[6];
        uint8_t channel;
#endif
      };

      struct Config
      {
        PhyMode_t phyMode;
        OutputPower_t outputPower;

#ifdef IOT_NODE_IP_STATIC
        InterfaceConfig interfaceConfig;
#endif

        Credentials credentials;
        Timings timings;
      };

      struct State
      {
        bool firstConnectionAttempted = false;
        bool firstConnectionSucceeded = false;
        bool isConnected = false;
        bool isReconnecting = false;
        bool shouldBeConnected = false;
        unsigned long disconnectionTime = 0;
        unsigned long maintenanceTime = 0;
        PhyMode_t phyMode;
        OutputPower_t outputPower;

#ifdef IOT_NODE_IP_STATIC
        InterfaceConfig interfaceConfig;
#endif

        Credentials credentials;
        Timings timings;
        EventListeners eventListeners;
        Callbacks callbacks;
      };

      class Class
      {
      private:
        State state;
        void wifiConnect();
        void handleConnected(String ssid, uint8_t *bssid, uint8_t channel);
        void handleDhcpTimeout();
        void handleDisconnected(String ssid, uint8_t *bssid, DisconnectReason_t reason);
        void handleGotIP(IPAddress ip, IPAddress gateway, IPAddress netmask);

      public:
        Class(Config config);
        void configDebug();
        void connect();
        void disconnect();
        bool isConnected();
        void onBeforeRestart(std::function<void()> callback);
        void onConnected(std::function<void()> callback);
        void onDhcpTimeout(std::function<void()> callback);
        void onDisconnected(std::function<void()> callback);
        void onGotIP(std::function<void(IPAddress ip)> callback);
        void onReconnect(std::function<void()> callback);
        void update();
        void debug(bool deep);
      };
    }

  } // section namespace
} // project namespace

#endif
