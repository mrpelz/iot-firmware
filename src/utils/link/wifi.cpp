#include "./wifi.h"

#ifdef IOT_NODE_LINK_WIFI

namespace IotNode {
namespace Utils {

namespace Link {
  Class::Class(Config config) {
    state.phyMode = config.phyMode;
    state.outputPower = config.outputPower;

    #ifdef IOT_NODE_IP_STATIC
      state.interfaceConfig = config.interfaceConfig;
    #endif

    state.credentials = config.credentials;
    state.timings = config.timings;

    #ifdef IOT_NODE_ESP8266
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
    #ifdef IOT_NODE_ESP32
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

    WiFi.persistent(false);

    WiFi.setAutoConnect(false);
    WiFi.setAutoReconnect(false);

    WiFi.mode(WIFI_STA);

    #ifdef IOT_NODE_ESP32
      WiFi.setSleep(false);
      WiFi.setHostname(IOT_NODE_NAME);
    #endif

    #ifdef IOT_NODE_ESP8266
      WiFi.setPhyMode(state.phyMode);
      WiFi.setSleepMode(WIFI_NONE_SLEEP);
      wifi_station_set_hostname(IOT_NODE_NAME);
    #endif

    if (state.outputPower != 0) {
      #ifdef IOT_NODE_ESP8266
        WiFi.setOutputPower(state.outputPower);
      #endif
      #ifdef IOT_NODE_ESP32
        WiFi.setTxPower(state.outputPower);
      #endif
    }
  }

  void Class::configDebug() {
    state.callbacks.debug("config.wifi.phy-mode", String(state.phyMode));
    state.callbacks.debug("config.wifi.output-power", String(state.outputPower));
    state.callbacks.debug("config.wifi.ssid", state.credentials.ssid);

    #ifdef IOT_NODE_ADVANCED_WIFI_CONFIG
      state.callbacks.debug("config.wifi.bssid", printMacAddress(state.credentials.bssid));
      state.callbacks.debug("config.wifi.channel", String(state.credentials.channel));
    #endif

    #ifdef IOT_NODE_IP_DHCP
      state.callbacks.debug("config.wifi.network-config.dhcp", "1");
    #else
      state.callbacks.debug("config.wifi.network-config.dhcp", "0");
      state.callbacks.debug("config.wifi.network-config.ip", state.interfaceConfig.ip.toString());
      state.callbacks.debug("config.wifi.network-config.gateway", state.interfaceConfig.gateway.toString());
      state.callbacks.debug("config.wifi.network-config.netmask", state.interfaceConfig.netmask.toString());
    #endif
  }

  void Class::connect() {
    state.shouldBeConnected = true;
  }

  void Class::disconnect() {
    state.shouldBeConnected = false;
  }

  void Class::handleConnected(String ssid, uint8_t *bssid, uint8_t channel) {
    state.isConnected = true;

    state.callbacks.debug("event", "wifi.connect");
    state.callbacks.debug("event.wifi.connect.ssid", ssid);
    state.callbacks.debug("event.wifi.connect.bssid", printMacAddress(bssid));
    state.callbacks.debug("event.wifi.connect.channel", String(channel));

    state.callbacks.connected();
  }

  void Class::handleDhcpTimeout() {
    state.callbacks.debug("event", "wifi.network-config.dhcp-timeout");

    state.callbacks.dhcpTimeout();
  }

  void Class::handleDisconnected(String ssid, uint8_t *bssid, DisconnectReason_t reason) {
    state.isConnected = false;

    state.callbacks.debug("event", "wifi.disconnect");
    state.callbacks.debug("event.wifi.disconnect.ssid", ssid);
    state.callbacks.debug("event.wifi.disconnect.bssid", printMacAddress(bssid));
    state.callbacks.debug("event.wifi.disconnect.reason", String(reason));

    state.callbacks.disconnected();
  }

  void Class::handleGotIP(IPAddress ip, IPAddress gateway, IPAddress netmask) {
    state.isConnected = true;

    state.callbacks.debug("event", "wifi.network-config");
    state.callbacks.debug("event.wifi.network-config.ip", ip.toString());
    state.callbacks.debug("event.wifi.network-config.gateway", gateway.toString());
    state.callbacks.debug("event.wifi.network-config.netmask", netmask.toString());

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

  void Class::setDebug(Log::Callback callback) {
    state.callbacks.debug = callback;

    configDebug();
  }

  void Class::update() {
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

  void Class::wifiConnect() {
    WiFi.disconnect();

    #ifdef IOT_NODE_ESP8266
      WiFi.forceSleepBegin();
      WiFi.forceSleepWake();
    #endif

    #ifdef IOT_NODE_ESP8266
      delay(LINK_DELAY);
    #endif
    #ifdef IOT_NODE_ESP32
      vTaskDelay(LINK_DELAY / portTICK_PERIOD_MS);
    #endif

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

    #ifdef IOT_NODE_IP_STATIC
      WiFi.config(
        state.interfaceConfig.ip,
        state.interfaceConfig.gateway,
        state.interfaceConfig.netmask,
        state.interfaceConfig.gateway
      );
    #endif
  }

  void Class::debug(bool deep) {
    if (deep) {
      #ifdef IOT_NODE_ESP8266
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

    #ifdef IOT_NODE_IP_DHCP
      IPAddress ip = WiFi.localIP();
      IPAddress gateway = WiFi.gatewayIP();
      IPAddress netmask = WiFi.subnetMask();

      state.callbacks.debug("info.wifi.network-config.ip", ip.toString());
      state.callbacks.debug("info.wifi.network-config.gateway", gateway.toString());
      state.callbacks.debug("info.wifi.network-config.netmask", netmask.toString());
    #endif
  }
}

} // section namespace
} // project namespace

#endif
