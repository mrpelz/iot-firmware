#pragma once

#ifdef IOT_NODE_LINK_ETH

#include <Arduino.h>
#include <ETH.h>

#include "./main.h"

namespace IotNode
{
  namespace Utils
  {

    namespace Link
    {
      struct Config
      {
#ifdef IOT_NODE_IP_STATIC
        InterfaceConfig interfaceConfig;
#endif

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

#ifdef IOT_NODE_IP_STATIC
        InterfaceConfig interfaceConfig;
#endif

        Timings timings;
        EventListeners eventListeners;
        Callbacks callbacks;
      };

      class Class
      {
      private:
        State state;
        void ethConnect();
        void handleConnected();
        void handleDhcpTimeout();
        void handleDisconnected(uint8_t reason);
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
