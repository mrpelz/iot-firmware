#ifndef _PERSISTENT_LINK_WIFI
#define _PERSISTENT_LINK_WIFI

#ifndef IOT_NODE_LINK_ETH

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
  typedef float OutputPower_t;
  typedef WiFiPhyMode_t PhyMode_t;
#endif
#ifdef ARDUINO_ARCH_ESP32
  typedef wifi_phy_rate_t PhyMode_t;
  typedef wifi_power_t OutputPower_t;
#endif

struct Credentials {
  String ssid;
  String password;

  #ifdef IOT_NODE_ADVANCED_WIFI_CONFIG
    uint8_t bssid[6];
    uint8_t channel;
  #endif
};

struct PersistentWiFiConfig {
  PhyMode_t phyMode;
  OutputPower_t outputPower;

  #ifndef IOT_NODE_DHCP
    InterfaceConfig interfaceConfig;
  #endif

  Credentials credentials;
  Timings timings;
};

struct PersistentWiFiState {
  bool firstConnectionAttempted = false;
  bool firstConnectionSucceeded = false;
  bool isConnected = false;
  bool isReconnecting = false;
  bool shouldBeConnected = false;
  unsigned long disconnectionTime = 0;
  unsigned long maintenanceTime = 0;
  PhyMode_t phyMode;
  OutputPower_t outputPower;

  #ifndef IOT_NODE_DHCP
    InterfaceConfig interfaceConfig;
  #endif

  Credentials credentials;
  Timings timings;
  EventListeners eventListeners;
  Callbacks callbacks;
};

class PersistentWiFi {
  private:
    PersistentWiFiState state;
    void configDebug();
    void wifiConnect();
    void handleConnected(String ssid, uint8_t *bssid, uint8_t channel);
    void handleDhcpTimeout();
    void handleDisconnected(String ssid, uint8_t *bssid, DisconnectReason_t reason);
    void handleGotIP(IPAddress ip, IPAddress gateway, IPAddress netmask);

  public:
    PersistentWiFi(PersistentWiFiConfig config);
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
