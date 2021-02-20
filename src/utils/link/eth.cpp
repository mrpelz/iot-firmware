#include "./eth.h"

#if defined(IOT_NODE_LINK_ETH) && defined(ARDUINO_ARCH_ESP32)

namespace Link {
  Class::Class(Config config) {
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

  void Class::configDebug() {
    #ifdef IOT_NODE_DHCP
      state.callbacks.debug("config.network-config.dhcp", "1");
    #else
      state.callbacks.debug("config.network-config.dhcp", "0");
      state.callbacks.debug("config.network-config.ip", state.interfaceConfig.ip.toString());
      state.callbacks.debug("config.network-config.gateway", state.interfaceConfig.gateway.toString());
      state.callbacks.debug("config.network-config.netmask", state.interfaceConfig.netmask.toString());
    #endif
  }

  void Class::connect() {
    state.shouldBeConnected = true;
    update();
  }

  void Class::disconnect() {
    state.shouldBeConnected = false;
    update();
  }

  void Class::ethConnect() {
    // yep, even with Ethernet, one must call WiFi.diconnect,
    // because reasons
    WiFi.disconnect();

    #ifdef IOT_NODE_WT32_ETH01
      ETH.begin(1, 16, 23, 18, ETH_PHY_LAN8720, ETH_CLOCK_GPIO0_IN);
    #else
      ETH.begin();
    #endif
  }

  void Class::handleConnected() {
    state.isConnected = true;

    state.callbacks.debug("event", "connect");

    update();

    state.callbacks.connected();
  }

  void Class::handleDhcpTimeout() {
    state.callbacks.debug("event", "network-config.dhcp-timeout");

    state.callbacks.dhcpTimeout();
  }

  void Class::handleDisconnected(uint8_t reason) {
    state.isConnected = false;

    state.callbacks.debug("event", "disconnect");
    state.callbacks.debug("event.disconnect.reason", String(reason));

    update();

    state.callbacks.disconnected();
  }

  void Class::handleGotIP(IPAddress ip, IPAddress gateway, IPAddress netmask) {
    state.callbacks.debug("event", "network-config");
    state.callbacks.debug("event.network-config.ip", ip.toString());
    state.callbacks.debug("event.network-config.gateway", gateway.toString());
    state.callbacks.debug("event.network-config.netmask", netmask.toString());

    state.callbacks.gotIP();
  }

  bool Class::isConnected() {
    return state.isConnected;
  }

  void Class::onBeforeRestart(std::function<void ()> callback) {
    state.callbacks.beforeRestart = callback;
  }

  void Class::onConnected(std::function<void ()> callback) {
    state.callbacks.connected = callback;
  }

  void Class::onDhcpTimeout(std::function<void ()> callback) {
    state.callbacks.dhcpTimeout = callback;
  }

  void Class::onDisconnected(std::function<void ()> callback) {
    state.callbacks.disconnected = callback;
  }

  void Class::onGotIP(std::function<void ()> callback) {
    state.callbacks.gotIP = callback;
  }

  void Class::onReconnect(std::function<void ()> callback) {
    state.callbacks.reconnect = callback;
  }

  void Class::setDebug(LoggingCallback callback) {
    state.callbacks.debug = callback;

    configDebug();
  }

  void Class::update() {
    unsigned long now = millis();

    // do not act if ETH is connected
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
        ethConnect();
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

      ethConnect();

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

  void Class::debug(bool deep) {
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
}

#endif
