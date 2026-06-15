#include "./wifi.h"

#ifdef IOT_NODE_LINK_WIFI

namespace IotNode::Utils::Link
{
  Class::Class(Config config)
  {
    state.phyMode = config.phyMode;
    state.outputPower = config.outputPower;

#ifdef IOT_NODE_IP_STATIC
    state.interfaceConfig = config.interfaceConfig;
#endif

    state.credentials = config.credentials;
    state.timings = config.timings;

    WiFi.persistent(false);
    WiFi.setAutoReconnect(false);

#ifdef IOT_NODE_ESP32
    WiFi.setSleep(false);
#endif

#ifdef IOT_NODE_ESP8266
    WiFi.setPhyMode(state.phyMode);
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
#endif

    if (state.outputPower != 0)
    {
#ifdef IOT_NODE_ESP8266
      WiFi.setOutputPower(state.outputPower);
#endif
#ifdef IOT_NODE_ESP32
      WiFi.setTxPower(state.outputPower);
#endif
    }
  }

  void Class::configDebug()
  {
#ifdef IOT_NODE_LOGGING
    Log::debug(fmt::format("config.wifi.phy-mode: {}", String(state.phyMode).c_str()));
    Log::debug(fmt::format("config.wifi.output-power: {}", String(state.outputPower).c_str()));
    Log::debug(fmt::format("config.wifi.ssid: {}", state.credentials.ssid));

#ifdef IOT_NODE_ADVANCED_WIFI_CONFIG
    Log::debug(fmt::format("config.wifi.bssid: {}", printMacAddress(state.credentials.bssid)));
    Log::debug(fmt::format("config.wifi.channel: {}", state.credentials.channel));
#endif

#ifdef IOT_NODE_IP_DHCP
    Log::debug("config.wifi.network-config.dhcp: 1");
#elif defined(IOT_NODE_IP_STATIC)
    Log::debug("config.wifi.network-config.dhcp: 0");
    Log::debug(fmt::format("config.wifi.network-config.ip: {}", state.interfaceConfig.ip.toString().c_str()));
    Log::debug(fmt::format("config.wifi.network-config.gateway: {}", state.interfaceConfig.gateway.toString().c_str()));
    Log::debug(fmt::format("config.wifi.network-config.netmask: {}", state.interfaceConfig.netmask.toString().c_str()));
#endif
#endif
  }

  void Class::connect()
  {
    state.shouldBeConnected = true;
  }

  void Class::disconnect()
  {
    state.shouldBeConnected = false;
  }

  void Class::handleConnected(String ssid, unsigned char *bssid, unsigned char channel)
  {
    state.isConnected = true;

#ifdef IOT_NODE_LOGGING
    Log::debug("event: wifi.connect");
    Log::debug(fmt::format("event.wifi.connect.ssid: {}", ssid.c_str()));
    Log::debug(fmt::format("event.wifi.connect.bssid: {}", printMacAddress(bssid)));
    Log::debug(fmt::format("event.wifi.connect.channel: {}", channel));
#endif

    state.callbacks.connected();
  }

  void Class::handleDhcpTimeout()
  {
#ifdef IOT_NODE_LOGGING
    Log::debug("event: wifi.network-config.dhcp-timeout");
#endif

    state.callbacks.dhcpTimeout();
  }

  void Class::handleDisconnected(String ssid, unsigned char *bssid, DisconnectReason_t reason)
  {
    state.isConnected = false;

#ifdef IOT_NODE_LOGGING
    Log::debug("event: wifi.disconnect");
    Log::debug(fmt::format("event.wifi.disconnect.ssid: {}", ssid.c_str()));
    Log::debug(fmt::format("event.wifi.disconnect.bssid: {}", printMacAddress(bssid)));
    Log::debug(fmt::format("event.wifi.disconnect.reason: {}", ::std::to_string(reason)));
#endif

    state.callbacks.disconnected();
  }

  void Class::handleGotIP(IPAddress ip, IPAddress gateway, IPAddress netmask)
  {
    state.isConnected = true;

#ifdef IOT_NODE_LOGGING
    Log::debug("event: wifi.network-config");
    Log::debug(fmt::format("event.wifi.network-config.ip: {}", ip.toString().c_str()));
    Log::debug(fmt::format("event.wifi.network-config.gateway: {}", gateway.toString().c_str()));
    Log::debug(fmt::format("event.wifi.network-config.netmask: {}", netmask.toString().c_str()));
#endif

    state.callbacks.gotIP(ip);
  }

  bool Class::isConnected()
  {
    return state.isConnected;
  }

  void Class::onBeforeRestart(::std::function<void()> callback)
  {
    state.callbacks.beforeRestart = callback;
  }

  void Class::onConnected(::std::function<void()> callback)
  {
    state.callbacks.connected = callback;
  }

  void Class::onDhcpTimeout(::std::function<void()> callback)
  {
    state.callbacks.dhcpTimeout = callback;
  }

  void Class::onDisconnected(::std::function<void()> callback)
  {
    state.callbacks.disconnected = callback;
  }

  void Class::onGotIP(::std::function<void(IPAddress ip)> callback)
  {
    state.callbacks.gotIP = callback;
  }

  void Class::onReconnect(::std::function<void()> callback)
  {
    state.callbacks.reconnect = callback;
  }

  void Class::update()
  {
    unsigned long now = millis();

    // do not act if WiFi is connected
    // set disconnection time to 0
    if (state.isConnected)
    {
      if (!state.shouldBeConnected)
      {
        WiFi.disconnect();
      }

      if (!state.firstConnectionSucceeded)
      {
        state.firstConnectionSucceeded = true;

#ifdef IOT_NODE_LOGGING
        Log::debug("info.wifi.first-connect: succeeded");
#endif

        debug(true);
      }

      state.isReconnecting = false;
      state.disconnectionTime = 0;

      unsigned long timeSinceWifiMaintenance = now - state.maintenanceTime;

      if (
          now > state.timings.runDebugEvery && timeSinceWifiMaintenance > state.timings.runDebugEvery)
      {
        state.maintenanceTime = now;

#ifdef IOT_NODE_LOGGING
        Log::debug(fmt::format("info.wifi.maintenance: {}", timeSinceWifiMaintenance));
#endif

        debug(false);
      }

      return;
    };

    if (!state.shouldBeConnected)
      return;

    if (!state.firstConnectionAttempted)
    {
      state.firstConnectionAttempted = true;

      if (!state.isReconnecting)
      {
#ifdef IOT_NODE_LOGGING
        Log::debug("info.wifi.first-connect: attempting");
#endif

        state.isReconnecting = true;
        wifiConnect();
      }

      return;
    }

    // as soon as WiFi becomes disconnected,
    // save time of disconnection
    if (
        !state.isReconnecting && state.disconnectionTime == 0)
    {
      state.disconnectionTime = now;

#ifdef IOT_NODE_LOGGING
      Log::debug(fmt::format("info.timing.disconnection: {}", now));
#endif

      return;
    }

    unsigned long timeSinceWifiDisconnect = now - state.disconnectionTime;

    if (
        !state.isReconnecting && timeSinceWifiDisconnect > state.timings.tryReconnectAfter)
    {
      state.isReconnecting = true;

#ifdef IOT_NODE_LOGGING
      Log::debug(fmt::format("info.wifi.attempt-reconnect: {}", timeSinceWifiDisconnect));
#endif

      wifiConnect();

      state.callbacks.reconnect();
    }

    if (timeSinceWifiDisconnect > state.timings.restartAfter)
    {
#ifdef IOT_NODE_LOGGING
      Log::debug(fmt::format("info.wifi.attempt-restart: {}", timeSinceWifiDisconnect));
#endif

      state.callbacks.beforeRestart();

      ESP.restart();

      while (true)
      {
        yield();
      }
    }
  }

  void Class::wifiConnect()
  {
#ifdef IOT_NODE_ESP8266
    state.eventListeners.onConnected = WiFi.onStationModeConnected(
        [&](WiFiEventStationModeConnected event)
        {
          handleConnected(event.ssid, event.bssid, event.channel);
        });

    state.eventListeners.onDHCPTimeout = WiFi.onStationModeDHCPTimeout(
        [&]()
        {
          handleDhcpTimeout();
        });

    state.eventListeners.onDisconnected = WiFi.onStationModeDisconnected(
        [&](WiFiEventStationModeDisconnected event)
        {
          handleDisconnected(event.ssid, event.bssid, event.reason);
        });

    state.eventListeners.onGotIP = WiFi.onStationModeGotIP(
        [&](WiFiEventStationModeGotIP event)
        {
          handleGotIP(event.ip, event.gw, event.mask);
        });
#endif
#ifdef IOT_NODE_ESP32
    state.eventListeners.onConnected = WiFi.onEvent(
        [&](arduino_event_id_t event, arduino_event_info_t info)
        {
          handleConnected(
              String((char *)info.wifi_sta_connected.ssid),
              info.wifi_sta_connected.bssid,
              info.wifi_sta_connected.channel);
        },
        ARDUINO_EVENT_WIFI_STA_CONNECTED);

    state.eventListeners.onDHCPTimeout = WiFi.onEvent(
        [&](arduino_event_id_t event, arduino_event_info_t info)
        {
          handleDhcpTimeout();
        },
        ARDUINO_EVENT_WIFI_STA_LOST_IP);

    state.eventListeners.onDisconnected = WiFi.onEvent(
        [&](arduino_event_id_t event, arduino_event_info_t info)
        {
          handleDisconnected(
              String((char *)info.wifi_sta_disconnected.ssid),
              info.wifi_sta_disconnected.bssid,
              info.wifi_sta_disconnected.reason);
        },
        ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

    state.eventListeners.onGotIP = WiFi.onEvent(
        [&](arduino_event_id_t event, arduino_event_info_t info)
        {
          handleGotIP(
              IPAddress(info.got_ip.ip_info.ip.addr),
              IPAddress(info.got_ip.ip_info.gw.addr),
              IPAddress(info.got_ip.ip_info.netmask.addr));
        },
        ARDUINO_EVENT_WIFI_STA_GOT_IP);
#endif

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

#ifdef IOT_NODE_IP_STATIC
    WiFi.config(
        state.interfaceConfig.ip,
        state.interfaceConfig.gateway,
        state.interfaceConfig.netmask,
        state.interfaceConfig.gateway);
#endif

#ifdef IOT_NODE_ESP32
    WiFi.setHostname(IOT_NODE_PIO_ENV);
#endif

#ifdef IOT_NODE_ESP8266
    wifi_station_set_hostname(IOT_NODE_PIO_ENV);
#endif

#ifdef IOT_NODE_ADVANCED_WIFI_CONFIG
    WiFi.begin(
        state.credentials.ssid.c_str(),
        state.credentials.password.c_str(),
        state.credentials.channel,
        state.credentials.bssid);
#else
    WiFi.begin(state.credentials.ssid.c_str(), state.credentials.password.c_str());
#endif

#ifdef IOT_NODE_ESP8266
    delay(LINK_DELAY);
#endif
#ifdef IOT_NODE_ESP32
    vTaskDelay(LINK_DELAY / portTICK_PERIOD_MS);
#endif

    WiFi.mode(WIFI_STA);
  }

  void Class::debug(bool deep)
  {
#ifdef IOT_NODE_LOGGING
    if (deep)
    {
#ifdef IOT_NODE_ESP8266
      Log::debug(fmt::format("info.wifi.phy-mode: {}", ::std::to_string(WiFi.getPhyMode())));
#endif

      Log::debug(fmt::format("info.wifi.ssid: {}", WiFi.SSID().c_str()));

#ifdef IOT_NODE_ADVANCED_WIFI_CONFIG
      Log::debug(fmt::format("info.wifi.bssid: {}", printMacAddress(WiFi.BSSID())));
      Log::debug(fmt::format("info.wifi.channel: {}", WiFi.channel()));
#endif
    }

#ifndef IOT_NODE_ADVANCED_WIFI_CONFIG
    Log::debug(fmt::format("info.wifi.bssid: {}", printMacAddress(WiFi.BSSID())));
    Log::debug(fmt::format("info.wifi.channel: {}", WiFi.channel()));
#endif

    Log::debug(fmt::format("info.wifi.rssi: {}", WiFi.RSSI()));

#ifdef IOT_NODE_IP_DHCP
    IPAddress ip = WiFi.localIP();
    IPAddress gateway = WiFi.gatewayIP();
    IPAddress netmask = WiFi.subnetMask();

    Log::debug(fmt::format("info.wifi.network-config.ip: {}", ip.toString().c_str()));
    Log::debug(fmt::format("info.wifi.network-config.gateway: {}", gateway.toString().c_str()));
    Log::debug(fmt::format("info.wifi.network-config.netmask: {}", netmask.toString().c_str()));
#endif
#endif
  }
}

#endif
