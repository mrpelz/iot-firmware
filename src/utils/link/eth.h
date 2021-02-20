#ifndef _LINK_ETH
#define _LINK_ETH

#if defined(IOT_NODE_LINK_ETH) && defined(ARDUINO_ARCH_ESP32)

#include <Arduino.h>
#include <ETH.h>
#include "./main.h"

namespace IotNode {

namespace Link {
  struct Config {
    #ifndef IOT_NODE_DHCP
      InterfaceConfig interfaceConfig;
    #endif

    Timings timings;
  };

  struct State {
    bool firstConnectionAttempted = false;
    bool firstConnectionSucceeded = false;
    bool isConnected = false;
    bool isReconnecting = false;
    bool shouldBeConnected = false;
    unsigned long disconnectionTime = 0;
    unsigned long maintenanceTime = 0;

    #ifndef IOT_NODE_DHCP
      InterfaceConfig interfaceConfig;
    #endif

    Timings timings;
    EventListeners eventListeners;
    Callbacks callbacks;
  };

  class Class {
    private:
      State state;
      void configDebug();
      void ethConnect();
      void handleConnected();
      void handleDhcpTimeout();
      void handleDisconnected(uint8_t reason);
      void handleGotIP(IPAddress ip, IPAddress gateway, IPAddress netmask);

    public:
      Class(Config config);
      void connect();
      void disconnect();
      bool isConnected();
      void onBeforeRestart(std::function<void ()> callback);
      void onConnected(std::function<void ()> callback);
      void onDhcpTimeout(std::function<void ()> callback);
      void onDisconnected(std::function<void ()> callback);
      void onGotIP(std::function<void ()> callback);
      void onReconnect(std::function<void ()> callback);
      void setDebug(Log::Callback callback);
      void update();
      void debug(bool deep);
  };
}

} // project namespace

#endif

#endif
