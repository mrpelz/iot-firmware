#include "./persistent-link.h"

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

PersistentLink::PersistentLink(PersistentLinkConfig config) {
  state.phyMode = config.phyMode;
  state.outputPower = config.outputPower;

  #ifndef IOT_NODE_DHCP
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

  #ifdef IOT_NODE_DHCP
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

  #ifndef IOT_NODE_DHCP
    WiFi.config(
      state.interfaceConfig.ip,
      state.interfaceConfig.gateway,
      state.interfaceConfig.netmask,
      state.interfaceConfig.gateway
    );
  #endif
}

void PersistentLink::configDebug() {
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

void PersistentLink::connect() {
  state.wifiShouldBeConnected = true;
  update();
}

void PersistentLink::disconnect() {
  state.wifiShouldBeConnected = false;
  update();
}

bool PersistentLink::isConnected() {
  return state.wifiIsConnected;
}

void PersistentLink::onAuthModeChanged(std::function<void (const WiFiEventStationModeAuthModeChanged &)> callback) {
  state.callbacks.authModeChanged = callback;
}

void PersistentLink::onBeforeRestart(std::function<void ()> callback) {
  state.callbacks.beforeRestart = callback;
}

void PersistentLink::onConnected(std::function<void (const WiFiEventStationModeConnected &)> callback) {
  state.callbacks.connected = callback;
}

#ifdef IOT_NODE_DHCP
  void PersistentLink::onDhcpTimeout(std::function<void ()> callback) {
    state.callbacks.dhcpTimeout = callback;
  }
#endif

void PersistentLink::onDisconnected(std::function<void (const WiFiEventStationModeDisconnected &)> callback) {
  state.callbacks.disconnected = callback;
}

void PersistentLink::onGotIP(std::function<void (const WiFiEventStationModeGotIP &)> callback) {
  state.callbacks.gotIP = callback;
}

void PersistentLink::onModeChange(std::function<void (const WiFiEventModeChange &)> callback) {
  state.callbacks.modeChange = callback;
}

void PersistentLink::onReconnect(std::function<void ()> callback) {
  state.callbacks.reconnect = callback;
}

void PersistentLink::setDebug(std::function<void (const String &, const String &)> callback) {
  state.callbacks.debug = callback;

  configDebug();
}

void PersistentLink::update() {
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

void PersistentLink::wifiConnect() {
  WiFi.disconnect();

  #ifdef IOT_NODE_ADVANCED_WIFI_CONFIG
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

void PersistentLink::wifiDebug(bool deep) {
  if (deep) {
    state.callbacks.debug("info.wifi.phy-mode", String(WiFi.getPhyMode()));
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

    if (ip.isSet()) state.callbacks.debug("info.wifi.network-config.ip", ip.toString());
    if (gateway.isSet()) state.callbacks.debug("info.wifi.network-config.gateway", gateway.toString());
    if (netmask.isSet()) state.callbacks.debug("info.wifi.network-config.netmask", netmask.toString());
  #endif
}
