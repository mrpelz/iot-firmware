#ifndef _PERSISTENT_LINK_ETH
#define _PERSISTENT_LINK_ETH

#ifdef IOT_NODE_LINK_ETH

#if defined(IOT_NODE_AP_ALGORE) || defined(IOT_NODE_AP_ELZAR) || defined(IOT_NODE_AP_RICHARDNIXON) || defined(IOT_NODE_AP_SPIROAGNEW)
  #define IOT_NODE_ADVANCED_WIFI_CONFIG
#endif

#include <Arduino.h>

#ifdef ARDUINO_ARCH_ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>
#endif

#include "./tools.h"

#ifdef ARDUINO_ARCH_ESP8266
  typedef WiFiEventHandler EventHandler_t;
  typedef float OutputPower_t;
  typedef WiFiDisconnectReason DisconnectReason_t;
  typedef WiFiPhyMode_t PhyMode_t;
#endif
#ifdef ARDUINO_ARCH_ESP32
  typedef wifi_event_id_t EventHandler_t;
  typedef uint8_t DisconnectReason_t;
  typedef wifi_phy_rate_t PhyMode_t;
  typedef wifi_power_t OutputPower_t;
#endif

#ifndef IOT_NODE_DHCP
  struct InterfaceConfig {
    IPAddress ip;
    IPAddress gateway;
    IPAddress netmask;
  };
#endif

struct WiFiCredentials {
  String ssid;
  String password;

  #ifdef IOT_NODE_ADVANCED_WIFI_CONFIG
    uint8_t bssid[6];
    uint8_t channel;
  #endif
};

struct Timings {
  unsigned long runWifiDebugEvery;
  unsigned long tryWifiReconnectAfter;
  unsigned long restartAfter;
};

struct EventListeners {
  EventHandler_t onStationModeConnected;
  EventHandler_t onStationModeDisconnected;
  EventHandler_t onStationModeGotIP;
  EventHandler_t onStationModeDHCPTimeout;
};

struct Callbacks {
  std::function<void (const String &, const String &)> debug =
    [](String key, String value) {};

  std::function<void ()> beforeRestart = []() {};
  std::function<void ()> reconnect = []() {};

  std::function<void ()> connected = []() {};
  std::function<void ()> dhcpTimeout = []() {};
  std::function<void ()> disconnected = []() {};
  std::function<void ()> gotIP = []() {};
};

struct PersistentLinkConfig {
  PhyMode_t phyMode;
  OutputPower_t outputPower;

  #ifndef IOT_NODE_DHCP
    InterfaceConfig interfaceConfig;
  #endif

  WiFiCredentials credentials;
  Timings timings;
};

struct PersistentLinkState {
  bool firstConnectionAttempted = false;
  bool firstConnectionSucceeded = false;
  bool wifiIsConnected = false;
  bool wifiIsReconnecting = false;
  bool wifiShouldBeConnected = false;
  unsigned long wifiDisconnectionTime = 0;
  unsigned long wifiMaintenanceTime = 0;
  PhyMode_t phyMode;
  OutputPower_t outputPower;

  #ifndef IOT_NODE_DHCP
    InterfaceConfig interfaceConfig;
  #endif

  WiFiCredentials credentials;
  Timings timings;
  EventListeners eventListeners;
  Callbacks callbacks;
};

class PersistentLink {
  private:
    PersistentLinkState state;
    void configDebug();
    void wifiConnect();
    void handleConnected(String ssid, uint8_t *bssid, uint8_t channel);
    void handleDhcpTimeout();
    void handleDisconnected(String ssid, uint8_t *bssid, DisconnectReason_t reason);
    void handleGotIP(IPAddress ip, IPAddress gateway, IPAddress netmask);

  public:
    PersistentLink(PersistentLinkConfig config);
    void connect();
    void disconnect();
    bool isConnected();
    void onBeforeRestart(std::function<void ()> callback);
    void onConnected(std::function<void ()> callback);
    void onDhcpTimeout(std::function<void ()> callback);
    void onDisconnected(std::function<void ()> callback);
    void onGotIP(std::function<void ()> callback);
    void onReconnect(std::function<void ()> callback);
    void setDebug(std::function<void (const String &, const String &)> callback);
    void update();
    void wifiDebug(bool deep);
};

#endif

#endif
