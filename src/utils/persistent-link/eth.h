#ifndef _PERSISTENT_LINK_ETH
#define _PERSISTENT_LINK_ETH

#if defined(IOT_NODE_LINK_ETH) && defined(ARDUINO_ARCH_ESP32)

#include <Arduino.h>
#include <ETH.h>
#include "./tools.h"

struct PersistentEthConfig {
  #ifndef IOT_NODE_DHCP
    InterfaceConfig interfaceConfig;
  #endif

  Timings timings;
};

struct PersistentEthState {
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

class PersistentEth {
  private:
    PersistentEthState state;
    void configDebug();
    void ethConnect();
    void handleConnected();
    void handleDhcpTimeout();
    void handleDisconnected(uint8_t reason);
    void handleGotIP(IPAddress ip, IPAddress gateway, IPAddress netmask);

  public:
    PersistentEth(PersistentEthConfig config);
    void connect();
    void disconnect();
    bool isConnected();
    void onBeforeRestart(std::function<void ()> callback);
    void onConnected(std::function<void ()> callback);
    void onDhcpTimeout(std::function<void ()> callback);
    void onDisconnected(std::function<void ()> callback);
    void onGotIP(std::function<void ()> callback);
    void onReconnect(std::function<void ()> callback);
    void setDebug(LoggingCallback callback);
    void update();
    void debug(bool deep);
};

#endif

#endif
