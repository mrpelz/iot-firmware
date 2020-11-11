#include "./wifi.h"

#ifndef IOT_NODE_LINK_ETH

PersistentWiFi::PersistentWiFi(PersistentWiFiConfig config) {
  state.phyMode = config.phyMode;
  state.outputPower = config.outputPower;

  #ifndef IOT_NODE_DHCP
    state.interfaceConfig = config.interfaceConfig;
  #endif

  state.credentials = config.credentials;
  state.timings = config.timings;

  #ifdef ARDUINO_ARCH_ESP8266
    state.eventListeners.onConnected = WiFi.onStationModeConnected(
      [&](WiFiEventStationModeConnected event) {
        handleConnected(event.ssid, event.bssid, event.channel);
      }
    );

    state.eventListeners.onDHCPTimeout = WiFi.onStationModeDHCPTimeout(
      [&]() {
        handleDhcpTimeout();
      }
    );
    
    state.eventListeners.onDisconnected = WiFi.onStationModeDisconnected(
      [&](WiFiEventStationModeDisconnected event) {
        handleDisconnected(event.ssid, event.bssid, event.reason);
      }
    );

    state.eventListeners.onGotIP = WiFi.onStationModeGotIP(
      [&](WiFiEventStationModeGotIP event) {
        handleGotIP(event.ip, event.gw, event.mask);
      }
    );
  #endif
  #ifdef ARDUINO_ARCH_ESP32
    state.eventListeners.onConnected = WiFi.onEvent(
      [&](system_event_id_t event, system_event_info_t info) {
        handleConnected(
          String((char *)info.connected.ssid),
          info.connected.bssid,
          info.connected.channel
        );
      },
      SYSTEM_EVENT_STA_CONNECTED
    );

    state.eventListeners.onDHCPTimeout = WiFi.onEvent(
      [&](system_event_id_t event, system_event_info_t info) {
        handleDhcpTimeout();
      },
      SYSTEM_EVENT_STA_LOST_IP
    );

    state.eventListeners.onDisconnected = WiFi.onEvent(
      [&](system_event_id_t event, system_event_info_t info) {
        handleDisconnected(
          String((char *)info.disconnected.ssid),
          info.disconnected.bssid,
          info.disconnected.reason
        );
      },
      SYSTEM_EVENT_STA_DISCONNECTED
    );

    state.eventListeners.onGotIP = WiFi.onEvent(
      [&](system_event_id_t event, system_event_info_t info) {
        handleGotIP(
          IPAddress(info.got_ip.ip_info.ip.addr),
          IPAddress(info.got_ip.ip_info.gw.addr),
          IPAddress(info.got_ip.ip_info.netmask.addr)
        );
      },
      SYSTEM_EVENT_STA_GOT_IP
    );
  #endif

  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);
  WiFi.setAutoConnect(false);
  WiFi.setAutoReconnect(false);

  #ifdef ARDUINO_ARCH_ESP8266
    WiFi.setPhyMode(state.phyMode);
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
  #endif

  if (state.outputPower != 0) {
    #ifdef ARDUINO_ARCH_ESP8266
      WiFi.setOutputPower(state.outputPower);
    #endif
    #ifdef ARDUINO_ARCH_ESP32
      WiFi.setTxPower(state.outputPower);
    #endif
  }

  #ifndef IOT_NODE_DHCP
    WiFi.config(
      state.interfaceConfig.ip,
      state.interfaceConfig.gateway,
      state.interfaceConfig.netmask,
      state.interfaceConfig.gateway
    );
  #endif
}

void PersistentWiFi::configDebug() {
  state.callbacks.debug("config.wifi.phy-mode", String(state.phyMode));
  state.callbacks.debug("config.wifi.output-power", String(state.outputPower));
  state.callbacks.debug("config.wifi.ssid", state.credentials.ssid);

  #ifdef IOT_NODE_ADVANCED_WIFI_CONFIG
    state.callbacks.debug("config.wifi.bssid", printMacAddress(state.credentials.bssid));
    state.callbacks.debug("config.wifi.channel", String(state.credentials.channel));
  #endif

  #ifdef IOT_NODE_DHCP
    state.callbacks.debug("config.wifi.network-config.dhcp", "1");
  #else
    state.callbacks.debug("config.wifi.network-config.dhcp", "0");
    state.callbacks.debug("config.wifi.network-config.ip", state.interfaceConfig.ip.toString());
    state.callbacks.debug("config.wifi.network-config.gateway", state.interfaceConfig.gateway.toString());
    state.callbacks.debug("config.wifi.network-config.netmask", state.interfaceConfig.netmask.toString());
  #endif
}

void PersistentWiFi::connect() {
  state.shouldBeConnected = true;
  update();
}

void PersistentWiFi::disconnect() {
  state.shouldBeConnected = false;
  update();
}

void PersistentWiFi::handleConnected(String ssid, uint8_t *bssid, uint8_t channel) {
  state.isConnected = true;

  state.callbacks.debug("event", "wifi.connect");
  state.callbacks.debug("event.wifi.connect.ssid", ssid);
  state.callbacks.debug("event.wifi.connect.bssid", printMacAddress(bssid));
  state.callbacks.debug("event.wifi.connect.channel", String(channel));

  update();

  state.callbacks.connected();
}

void PersistentWiFi::handleDhcpTimeout() {
  state.callbacks.debug("event", "wifi.network-config.dhcp-timeout");

  state.callbacks.dhcpTimeout();
}

void PersistentWiFi::handleDisconnected(String ssid, uint8_t *bssid, DisconnectReason_t reason) {
  state.isConnected = false;

  state.callbacks.debug("event", "wifi.disconnect");
  state.callbacks.debug("event.wifi.disconnect.ssid", ssid);
  state.callbacks.debug("event.wifi.disconnect.bssid", printMacAddress(bssid));
  state.callbacks.debug("event.wifi.disconnect.reason", String(reason));

  update();

  state.callbacks.disconnected();
}

void PersistentWiFi::handleGotIP(IPAddress ip, IPAddress gateway, IPAddress netmask) {
  state.callbacks.debug("event", "wifi.network-config");
  state.callbacks.debug("event.wifi.network-config.ip", ip.toString());
  state.callbacks.debug("event.wifi.network-config.gateway", gateway.toString());
  state.callbacks.debug("event.wifi.network-config.netmask", netmask.toString());

  state.callbacks.gotIP();
}

bool PersistentWiFi::isConnected() {
  return state.isConnected;
}

void PersistentWiFi::onBeforeRestart(std::function<void ()> callback) {
  state.callbacks.beforeRestart = callback;
}

void PersistentWiFi::onConnected(std::function<void ()> callback) {
  state.callbacks.connected = callback;
}

void PersistentWiFi::onDhcpTimeout(std::function<void ()> callback) {
  state.callbacks.dhcpTimeout = callback;
}

void PersistentWiFi::onDisconnected(std::function<void ()> callback) {
  state.callbacks.disconnected = callback;
}

void PersistentWiFi::onGotIP(std::function<void ()> callback) {
  state.callbacks.gotIP = callback;
}

void PersistentWiFi::onReconnect(std::function<void ()> callback) {
  state.callbacks.reconnect = callback;
}

void PersistentWiFi::setDebug(std::function<void (const String &, const String &)> callback) {
  state.callbacks.debug = callback;

  configDebug();
}

void PersistentWiFi::update() {
  unsigned long now = millis();

  // do not act if WiFi is connected
  // set disconnection time to 0
  if (state.isConnected) {
    if (!state.shouldBeConnected) {
      WiFi.disconnect();
    }

    if (!state.firstConnectionSucceeded) {
      state.firstConnectionSucceeded = true;

      state.callbacks.debug("info.wifi.first-connect", "succeeded");
      debug(true);
    }

    state.isReconnecting = false;
    state.disconnectionTime = 0;

    unsigned long timeSinceWifiMaintenance = now - state.maintenanceTime;

    if (
      now > state.timings.runDebugEvery
      && timeSinceWifiMaintenance > state.timings.runDebugEvery
    ) {
      state.maintenanceTime = now;

      state.callbacks.debug("info.wifi.maintenance", String(timeSinceWifiMaintenance));
      debug(false);
    }

    return;
  };

  if (!state.shouldBeConnected) return;

  if (!state.firstConnectionAttempted) {
    state.firstConnectionAttempted = true;

    if (!state.isReconnecting) {
      state.callbacks.debug("info.wifi.first-connect", "attempting");

      state.isReconnecting = true;
      wifiConnect();
    }

    return;
  }

  // as soon as WiFi becomes disconnected,
  // save time of disconnection
  if (
    !state.isReconnecting
    && state.disconnectionTime == 0
  ) {
    state.disconnectionTime = now;

    state.callbacks.debug("info.timing.disconnection", String(now));

    return;
  }

  unsigned long timeSinceWifiDisconnect = now - state.disconnectionTime;

  if (
    !state.isReconnecting
    && timeSinceWifiDisconnect > state.timings.tryReconnectAfter
  ) {
    state.isReconnecting = true;

    state.callbacks.debug("info.wifi.attempt-reconnect", String(timeSinceWifiDisconnect));

    wifiConnect();

    state.callbacks.reconnect();
  }

  if (timeSinceWifiDisconnect > state.timings.restartAfter) {
    state.callbacks.debug("info.wifi.attempt-restart", String(timeSinceWifiDisconnect));

    state.callbacks.beforeRestart();

    ESP.restart();

    while (true) {
      yield();
    }
  }
}

void PersistentWiFi::wifiConnect() {
  WiFi.disconnect();

  #ifdef IOT_NODE_ADVANCED_WIFI_CONFIG
    WiFi.begin(
      state.credentials.ssid.c_str(),
      state.credentials.password.c_str(),
      state.credentials.channel,
      state.credentials.bssid
    );
  #else
    WiFi.begin(state.credentials.ssid.c_str(), state.credentials.password.c_str());
  #endif
}

void PersistentWiFi::debug(bool deep) {
  if (deep) {
    #ifdef ARDUINO_ARCH_ESP8266
      state.callbacks.debug("info.wifi.phy-mode", String(WiFi.getPhyMode()));
    #endif

    state.callbacks.debug("info.wifi.ssid", WiFi.SSID());

    #ifdef IOT_NODE_ADVANCED_WIFI_CONFIG
      state.callbacks.debug("info.wifi.bssid", printMacAddress(WiFi.BSSID()));
      state.callbacks.debug("info.wifi.channel", String(WiFi.channel()));
    #endif
  }

  #ifndef IOT_NODE_ADVANCED_WIFI_CONFIG
    state.callbacks.debug("info.wifi.bssid", printMacAddress(WiFi.BSSID()));
    state.callbacks.debug("info.wifi.channel", String(WiFi.channel()));
  #endif

  state.callbacks.debug("info.wifi.rssi", String(WiFi.RSSI()));

  #ifdef IOT_NODE_DHCP
    IPAddress ip = WiFi.localIP();
    IPAddress gateway = WiFi.gatewayIP();
    IPAddress netmask = WiFi.subnetMask();

    state.callbacks.debug("info.wifi.network-config.ip", ip.toString());
    state.callbacks.debug("info.wifi.network-config.gateway", gateway.toString());
    state.callbacks.debug("info.wifi.network-config.netmask", netmask.toString());
  #endif
}

#endif
