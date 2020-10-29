#include <Arduino.h>
#include <ESP8266WiFi.h>

#ifndef USE_DHCP
  struct InterfaceConfig {
    IPAddress ip;
    IPAddress gateway;
    IPAddress netmask;
  };
#endif

struct WiFiCredentials {
  String ssid;
  String password;

  #ifdef USE_ADVANCED_WIFI_CONFIG
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

  #ifdef USE_DHCP
    WiFiEventHandler onStationModeDHCPTimeout;
  #endif

  WiFiEventHandler onStationModeDisconnected;
  WiFiEventHandler onStationModeGotIP;
  WiFiEventHandler onWiFiModeChange;
};

struct Callbacks {
  std::function<void (const String &, const String &)> debug =
    [](String key, String value) {};
  std::function<void (const WiFiEventStationModeAuthModeChanged &)> authModeChanged =
    [](WiFiEventStationModeAuthModeChanged event) {};
  std::function<void (const WiFiEventStationModeConnected &)> connected =
    [](WiFiEventStationModeConnected event) {};
  
  #ifdef USE_DHCP
    std::function<void ()> dhcpTimeout =
      []() {};
  #endif

  std::function<void (const WiFiEventStationModeDisconnected &)> disconnected =
    [](WiFiEventStationModeDisconnected event) {};
  std::function<void (const WiFiEventStationModeGotIP &)> gotIP =
    [](WiFiEventStationModeGotIP event) {};
  std::function<void (const WiFiEventModeChange &)> modeChange =
    [](WiFiEventModeChange event) {};
};

struct PersistentWifiConfig {
  WiFiPhyMode_t phyMode;
  float outputPower;

  #ifndef USE_DHCP
    InterfaceConfig interfaceConfig;
  #endif

  WiFiCredentials credentials;
  Timings timings;
};

struct State {
  bool firstConnectionAttempted = false;
  bool firstConnectionSucceeded = false;
  bool wifiIsConnected = false;
  bool wifiIsReconnecting = false;
  bool wifiShouldBeConnected = false;
  unsigned long wifiDisconnectionTime = 0;
  unsigned long wifiMaintenanceTime = 0;
  WiFiPhyMode_t phyMode;
  float outputPower;

  #ifndef USE_DHCP
    InterfaceConfig interfaceConfig;
  #endif

  WiFiCredentials credentials;
  Timings timings;
  EventListeners eventListeners;
  Callbacks callbacks;
};

class PersistentWiFi {
  private:
    State state;

    void configDebug() {
      state.callbacks.debug("config.wifi.phy-mode", String(state.phyMode));
      state.callbacks.debug("config.wifi.output-power", String(state.outputPower));
      state.callbacks.debug("config.wifi.ssid", state.credentials.ssid);

      #ifdef USE_ADVANCED_WIFI_CONFIG
        state.callbacks.debug("config.wifi.bssid", printMacAddress(state.credentials.bssid));
        state.callbacks.debug("config.wifi.channel", String(state.credentials.channel));
      #endif

      #ifdef USE_DHCP
        state.callbacks.debug("config.wifi.network-config.dhcp", "1");
      #else
        state.callbacks.debug("config.wifi.network-config.dhcp", "0");
        state.callbacks.debug("config.wifi.network-config.ip", state.interfaceConfig.ip.toString());
        state.callbacks.debug("config.wifi.network-config.gateway", state.interfaceConfig.gateway.toString());
        state.callbacks.debug("config.wifi.network-config.netmask", state.interfaceConfig.netmask.toString());
      #endif
    }

    String printMacAddress(uint8_t input[6]) {
      char result[18];

      snprintf(
        result,
        sizeof(result),
        "%02x:%02x:%02x:%02x:%02x:%02x",
        input[0],
        input[1],
        input[2],
        input[3],
        input[4],
        input[5]
      );

      String output(result);
      output.toUpperCase();

      return output;
    }

    void wifiConnect() {
      WiFi.disconnect();

      #ifdef USE_ADVANCED_WIFI_CONFIG
        WiFi.begin(
          state.credentials.ssid,
          state.credentials.password,
          state.credentials.channel,
          state.credentials.bssid
        );
      #else
        WiFi.begin(state.credentials.ssid, state.credentials.password);
      #endif
    }

  public:
    PersistentWiFi(PersistentWifiConfig config) {
      state.phyMode = config.phyMode;
      state.outputPower = config.outputPower;

      #ifndef USE_DHCP
        state.interfaceConfig = config.interfaceConfig;
      #endif

      state.credentials = config.credentials;
      state.timings = config.timings;

      state.eventListeners.onStationModeAuthModeChanged = WiFi.onStationModeAuthModeChanged(
        [&](WiFiEventStationModeAuthModeChanged event) {
          state.callbacks.debug("event", "wifi.change.auth-mode");
          state.callbacks.debug("event.wifi.change.auth-mode.old", String(event.oldMode));
          state.callbacks.debug("event.wifi.change.auth-mode.new", String(event.newMode));

          state.callbacks.authModeChanged(event);
        }
      );

      state.eventListeners.onStationModeConnected = WiFi.onStationModeConnected(
        [&](WiFiEventStationModeConnected event) {
          state.wifiIsConnected = true;

          state.callbacks.debug("event", "wifi.connect");
          state.callbacks.debug("event.wifi.connect.ssid", event.ssid);
          state.callbacks.debug("event.wifi.connect.channel", String(event.channel));

          update();

          state.callbacks.connected(event);
        }
      );

      #ifdef USE_DHCP
        state.eventListeners.onStationModeDHCPTimeout = WiFi.onStationModeDHCPTimeout(
          [&]() {
            state.callbacks.debug("event", "wifi.network-config.dhcp-timeout");

            state.callbacks.dhcpTimeout();
          }
        );
      #endif
      
      state.eventListeners.onStationModeDisconnected = WiFi.onStationModeDisconnected(
        [&](WiFiEventStationModeDisconnected event) {
          state.wifiIsConnected = false;

          state.callbacks.debug("event", "wifi.disconnect");
          state.callbacks.debug("event.wifi.disconnect.ssid", event.ssid);
          state.callbacks.debug("event.wifi.disconnect.reason", String(event.reason));

          update();

          state.callbacks.disconnected(event);
        }
      );

      state.eventListeners.onStationModeGotIP = WiFi.onStationModeGotIP(
        [&](WiFiEventStationModeGotIP event) {
          state.callbacks.debug("event", "wifi.network-config");
          state.callbacks.debug("event.wifi.network-config.ip", event.ip.toString());
          state.callbacks.debug("event.wifi.network-config.gateway", event.gw.toString());
          state.callbacks.debug("event.wifi.network-config.netmask", event.mask.toString());

          state.callbacks.gotIP(event);
        }
      );

      state.eventListeners.onWiFiModeChange = WiFi.onWiFiModeChange(
        [&](WiFiEventModeChange event) {
          state.callbacks.debug("event", "wifi.change.mode");
          state.callbacks.debug("event.wifi.change.mode.old", String(event.oldMode));
          state.callbacks.debug("event.wifi.change.mode.new", String(event.newMode));

          state.callbacks.modeChange(event);
        }
      );

      WiFi.mode(WIFI_STA);
      WiFi.persistent(false);
      WiFi.setAutoConnect(false);
      WiFi.setAutoReconnect(false);
      WiFi.setPhyMode(state.phyMode);

      if (state.outputPower != 0) {
        WiFi.setOutputPower(state.outputPower);
      }

      #ifndef USE_DHCP
        WiFi.config(
          state.interfaceConfig.ip,
          state.interfaceConfig.gateway,
          state.interfaceConfig.netmask,
          state.interfaceConfig.gateway
        );
      #endif
    }

    void connect() {
      state.wifiShouldBeConnected = true;
      update();
    }

    void disconnect() {
      state.wifiShouldBeConnected = false;
      update();
    }

    bool isConnected() {
      return state.wifiIsConnected;
    }

    void onAuthModeChanged(std::function<void (const WiFiEventStationModeAuthModeChanged &)> callback) {
      state.callbacks.authModeChanged = callback;
    }

    void onConnected(std::function<void (const WiFiEventStationModeConnected &)> callback) {
      state.callbacks.connected = callback;
    }

    #ifdef USE_DHCP
      void onDhcpTimeout(std::function<void ()> callback) {
        state.callbacks.dhcpTimeout = callback;
      }
    #endif

    void onDisconnected(std::function<void (const WiFiEventStationModeDisconnected &)> callback) {
      state.callbacks.disconnected = callback;
    }

    void onGotIP(std::function<void (const WiFiEventStationModeGotIP &)> callback) {
      state.callbacks.gotIP = callback;
    }

    void onModeChange(std::function<void (const WiFiEventModeChange &)> callback) {
      state.callbacks.modeChange = callback;
    }

    void setDebug(std::function<void (const String &, const String &)> callback) {
      state.callbacks.debug = callback;

      configDebug();
    }

    void update() {
      unsigned long now = millis();

      // do not act if WiFi is connected
      // set disconnection time to 0
      if (state.wifiIsConnected) {
        if (!state.wifiShouldBeConnected) {
          WiFi.disconnect();
        }

        if (!state.firstConnectionSucceeded) {
          state.firstConnectionSucceeded = true;

          state.callbacks.debug("info.wifi.first-connect", "succeeded");
          wifiDebug(true);
        }

        state.wifiIsReconnecting = false;
        state.wifiDisconnectionTime = 0;

        unsigned long timeSinceWifiMaintenance = now - state.wifiMaintenanceTime;

        if (
          now > state.timings.runWifiDebugEvery
          && timeSinceWifiMaintenance > state.timings.runWifiDebugEvery
        ) {
          state.wifiMaintenanceTime = now;

          state.callbacks.debug("info.wifi.maintenance", String(timeSinceWifiMaintenance));
          wifiDebug(false);
        }

        return;
      };

      if (!state.wifiShouldBeConnected) return;

      if (!state.firstConnectionAttempted) {
        state.firstConnectionAttempted = true;

        if (!state.wifiIsReconnecting) {
          state.callbacks.debug("info.wifi.first-connect", "attempting");

          state.wifiIsReconnecting = true;
          wifiConnect();
        }

        return;
      }

      // as soon as WiFi becomes disconnected,
      // save time of disconnection
      if (
        !state.wifiIsReconnecting
        && state.wifiDisconnectionTime == 0
      ) {
        state.wifiDisconnectionTime = now;

        state.callbacks.debug("info.timing.disconnection", String(now));

        return;
      }

      unsigned long timeSinceWifiDisconnect = now - state.wifiDisconnectionTime;

      if (
        !state.wifiIsReconnecting
        && timeSinceWifiDisconnect > state.timings.tryWifiReconnectAfter
      ) {
        state.wifiIsReconnecting = true;

        state.callbacks.debug("info.wifi.attempt-reconnect", String(timeSinceWifiDisconnect));

        wifiConnect();
      }

      if (timeSinceWifiDisconnect > state.timings.restartAfter) {
        state.callbacks.debug("info.wifi.attempt-restart", String(timeSinceWifiDisconnect));

        ESP.restart();

        while (true) {
          yield();
        }
      }
    }

    void wifiDebug(bool deep) {
      if (deep) {
        state.callbacks.debug("info.wifi.phy-mode", String(WiFi.getPhyMode()));
        state.callbacks.debug("info.wifi.ssid", WiFi.SSID());

        #ifdef USE_ADVANCED_WIFI_CONFIG
          state.callbacks.debug("info.wifi.bssid", printMacAddress(WiFi.BSSID()));
          state.callbacks.debug("info.wifi.channel", String(WiFi.channel()));
        #endif
      }

      #ifndef USE_ADVANCED_WIFI_CONFIG
        state.callbacks.debug("info.wifi.bssid", printMacAddress(WiFi.BSSID()));
        state.callbacks.debug("info.wifi.channel", String(WiFi.channel()));
      #endif

      state.callbacks.debug("info.wifi.rssi", String(WiFi.RSSI()));

      #ifdef USE_DHCP
        state.callbacks.debug("info.wifi.network-config.ip", WiFi.localIP().toString());
        state.callbacks.debug("info.wifi.network-config.gateway", WiFi.gatewayIP().toString());
        state.callbacks.debug("info.wifi.network-config.netmask", WiFi.subnetMask().toString());
      #endif
    }
};
