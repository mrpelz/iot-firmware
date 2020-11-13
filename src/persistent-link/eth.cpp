#include "./eth.h"

#if defined(IOT_NODE_LINK_ETH) && defined(ARDUINO_ARCH_ESP32)

PersistentEth::PersistentEth(PersistentEthConfig config) {
  #ifndef IOT_NODE_DHCP
    state.interfaceConfig = config.interfaceConfig;
  #endif

  state.timings = config.timings;

  state.eventListeners.onConnected = WiFi.onEvent(
    [&](system_event_id_t event, system_event_info_t info) {
      handleConnected();
    },
    SYSTEM_EVENT_ETH_CONNECTED
  );

  state.eventListeners.onDHCPTimeout = WiFi.onEvent(
    [&](system_event_id_t event, system_event_info_t info) {
      handleDhcpTimeout();
    },
    SYSTEM_EVENT_STA_LOST_IP
  );

  state.eventListeners.onDisconnected = WiFi.onEvent(
    [&](system_event_id_t event, system_event_info_t info) {
      handleDisconnected(info.disconnected.reason);
    },
    SYSTEM_EVENT_ETH_DISCONNECTED
  );

  state.eventListeners.onGotIP = WiFi.onEvent(
    [&](system_event_id_t event, system_event_info_t info) {
      handleGotIP(
        IPAddress(info.got_ip.ip_info.ip.addr),
        IPAddress(info.got_ip.ip_info.gw.addr),
        IPAddress(info.got_ip.ip_info.netmask.addr)
      );
    },
    SYSTEM_EVENT_ETH_GOT_IP
  );

  #ifndef IOT_NODE_DHCP
    ETH.config(
      state.interfaceConfig.ip,
      state.interfaceConfig.gateway,
      state.interfaceConfig.netmask,
      state.interfaceConfig.gateway
    );
  #endif
}

void PersistentEth::configDebug() {
  #ifdef IOT_NODE_DHCP
    state.callbacks.debug("config.network-config.dhcp", "1");
  #else
    state.callbacks.debug("config.network-config.dhcp", "0");
    state.callbacks.debug("config.network-config.ip", state.interfaceConfig.ip.toString());
    state.callbacks.debug("config.network-config.gateway", state.interfaceConfig.gateway.toString());
    state.callbacks.debug("config.network-config.netmask", state.interfaceConfig.netmask.toString());
  #endif
}

void PersistentEth::connect() {
  state.shouldBeConnected = true;
  update();
}

void PersistentEth::disconnect() {
  state.shouldBeConnected = false;
  update();
}

void PersistentEth::handleConnected() {
  state.isConnected = true;

  state.callbacks.debug("event", "connect");

  update();

  state.callbacks.connected();
}

void PersistentEth::handleDhcpTimeout() {
  state.callbacks.debug("event", "network-config.dhcp-timeout");

  state.callbacks.dhcpTimeout();
}

void PersistentEth::handleDisconnected(uint8_t reason) {
  state.isConnected = false;

  state.callbacks.debug("event", "disconnect");
  state.callbacks.debug("event.disconnect.reason", String(reason));

  update();

  state.callbacks.disconnected();
}

void PersistentEth::handleGotIP(IPAddress ip, IPAddress gateway, IPAddress netmask) {
  state.callbacks.debug("event", "network-config");
  state.callbacks.debug("event.network-config.ip", ip.toString());
  state.callbacks.debug("event.network-config.gateway", gateway.toString());
  state.callbacks.debug("event.network-config.netmask", netmask.toString());

  state.callbacks.gotIP();
}

bool PersistentEth::isConnected() {
  return state.isConnected;
}

void PersistentEth::onBeforeRestart(std::function<void ()> callback) {
  state.callbacks.beforeRestart = callback;
}

void PersistentEth::onConnected(std::function<void ()> callback) {
  state.callbacks.connected = callback;
}

void PersistentEth::onDhcpTimeout(std::function<void ()> callback) {
  state.callbacks.dhcpTimeout = callback;
}

void PersistentEth::onDisconnected(std::function<void ()> callback) {
  state.callbacks.disconnected = callback;
}

void PersistentEth::onGotIP(std::function<void ()> callback) {
  state.callbacks.gotIP = callback;
}

void PersistentEth::onReconnect(std::function<void ()> callback) {
  state.callbacks.reconnect = callback;
}

void PersistentEth::setDebug(LoggingCallback callback) {
  state.callbacks.debug = callback;

  configDebug();
}

void PersistentEth::update() {
  unsigned long now = millis();

  // do not act if WiFi is connected
  // set disconnection time to 0
  if (state.isConnected) {
    if (!state.shouldBeConnected) {
      WiFi.disconnect();
    }

    if (!state.firstConnectionSucceeded) {
      state.firstConnectionSucceeded = true;

      state.callbacks.debug("info.first-connect", "succeeded");
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

      state.callbacks.debug("info.maintenance", String(timeSinceWifiMaintenance));
      debug(false);
    }

    return;
  };

  if (!state.shouldBeConnected) return;

  if (!state.firstConnectionAttempted) {
    state.firstConnectionAttempted = true;

    if (!state.isReconnecting) {
      state.callbacks.debug("info.first-connect", "attempting");

      state.isReconnecting = true;
      ETH.begin();
    }

    return;
  }

  // as soon as ETH becomes disconnected,
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

    state.callbacks.debug("info.attempt-reconnect", String(timeSinceWifiDisconnect));

    ETH.begin();

    state.callbacks.reconnect();
  }

  if (timeSinceWifiDisconnect > state.timings.restartAfter) {
    state.callbacks.debug("info.attempt-restart", String(timeSinceWifiDisconnect));

    state.callbacks.beforeRestart();

    ESP.restart();

    while (true) {
      yield();
    }
  }
}

void PersistentEth::debug(bool deep) {
  state.callbacks.debug("info.eth.link-speed", String(ETH.linkSpeed()));

  #ifdef IOT_NODE_DHCP
    IPAddress ip = ETH.localIP();
    IPAddress gateway = ETH.gatewayIP();
    IPAddress netmask = ETH.subnetMask();

    state.callbacks.debug("info.network-config.ip", ip.toString());
    state.callbacks.debug("info.network-config.gateway", gateway.toString());
    state.callbacks.debug("info.network-config.netmask", netmask.toString());
  #endif
}

#endif
