#ifndef _PERSISTENT_LINK
#define _PERSISTENT_LINK

#include <Arduino.h>
#include <ESP8266WiFi.h>

String printMacAddress(uint8_t input[6]);

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
  WiFiEventHandler onStationModeAuthModeChanged;
  WiFiEventHandler onStationModeConnected;
  WiFiEventHandler onStationModeDisconnected;
  WiFiEventHandler onStationModeGotIP;
  WiFiEventHandler onWiFiModeChange;

  #ifdef IOT_NODE_DHCP
    WiFiEventHandler onStationModeDHCPTimeout;
  #endif
};

struct Callbacks {
  std::function<void (const String &, const String &)> debug =
    [](String key, String value) {};

  std::function<void ()> beforeRestart =
    []() {};
  std::function<void ()> reconnect =
    []() {};

  std::function<void (const WiFiEventStationModeAuthModeChanged &)> authModeChanged =
    [](WiFiEventStationModeAuthModeChanged event) {};
  std::function<void (const WiFiEventStationModeConnected &)> connected =
    [](WiFiEventStationModeConnected event) {};
  std::function<void (const WiFiEventStationModeDisconnected &)> disconnected =
    [](WiFiEventStationModeDisconnected event) {};
  std::function<void (const WiFiEventStationModeGotIP &)> gotIP =
    [](WiFiEventStationModeGotIP event) {};
  std::function<void (const WiFiEventModeChange &)> modeChange =
    [](WiFiEventModeChange event) {};

  #ifdef IOT_NODE_DHCP
    std::function<void ()> dhcpTimeout =
      []() {};
  #endif
};

struct PersistentLinkConfig {
  WiFiPhyMode_t phyMode;
  float outputPower;

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
  WiFiPhyMode_t phyMode;
  float outputPower;

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

  public:
    PersistentLink(PersistentLinkConfig config);
    void connect();
    void disconnect();
    bool isConnected();
    void onAuthModeChanged(std::function<void (const WiFiEventStationModeAuthModeChanged &)> callback);
    void onBeforeRestart(std::function<void ()> callback);
    void onConnected(std::function<void (const WiFiEventStationModeConnected &)> callback);
    #ifdef IOT_NODE_DHCP
      void onDhcpTimeout(std::function<void ()> callback);
    #endif
    void onDisconnected(std::function<void (const WiFiEventStationModeDisconnected &)> callback);
    void onGotIP(std::function<void (const WiFiEventStationModeGotIP &)> callback);
    void onModeChange(std::function<void (const WiFiEventModeChange &)> callback);
    void onReconnect(std::function<void ()> callback);
    void setDebug(std::function<void (const String &, const String &)> callback);
    void update();
    void wifiDebug(bool deep);
};

#endif
