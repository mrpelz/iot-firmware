#include "./eth.h"

#ifdef IOT_NODE_LINK_ETH

namespace IotNode
{
  namespace Utils
  {

    namespace Link
    {
      Class::Class(Config config)
      {
#ifdef IOT_NODE_IP_STATIC
        state.interfaceConfig = config.interfaceConfig;
#endif

        state.timings = config.timings;

        WiFi.mode(WIFI_OFF);
        btStop();
      }

      void Class::configDebug()
      {
#ifdef IOT_NODE_LOGGING
#ifdef IOT_NODE_IP_DHCP
        Log::debug("config.network-config.dhcp: 1");
#else
        Log::debug("config.network-config.dhcp: 0");
        Log::debug("config.network-config.ip", state.interfaceConfig.ip.toString());
        Log::debug("config.network-config.gateway", state.interfaceConfig.gateway.toString());
        Log::debug("config.network-config.netmask", state.interfaceConfig.netmask.toString());
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

      void Class::ethConnect()
      {
        state.eventListeners.onConnected = WiFi.onEvent(
            [&](arduino_event_id_t event, arduino_event_info_t info)
            {
              handleConnected();
            },
            ARDUINO_EVENT_ETH_CONNECTED);

        state.eventListeners.onDHCPTimeout = WiFi.onEvent(
            [&](arduino_event_id_t event, arduino_event_info_t info)
            {
              handleDhcpTimeout();
            },
            ARDUINO_EVENT_WIFI_STA_LOST_IP);

        state.eventListeners.onDisconnected = WiFi.onEvent(
            [&](arduino_event_id_t event, arduino_event_info_t info)
            {
              handleDisconnected(info.prov_fail_reason);
            },
            ARDUINO_EVENT_ETH_DISCONNECTED);

        state.eventListeners.onGotIP = WiFi.onEvent(
            [&](arduino_event_id_t event, arduino_event_info_t info)
            {
              handleGotIP(
                  IPAddress(info.got_ip.ip_info.ip.addr),
                  IPAddress(info.got_ip.ip_info.gw.addr),
                  IPAddress(info.got_ip.ip_info.netmask.addr));
            },
            ARDUINO_EVENT_ETH_GOT_IP);

        // yep, even with Ethernet, one must call WiFi.diconnect,
        // because reasons
        WiFi.disconnect();

        vTaskDelay(LINK_DELAY / portTICK_PERIOD_MS);

#ifdef IOT_NODE_WT32_ETH01
        ETH.begin(ETH_PHY_LAN8720, ETH_PHY_ADDR_AUTO, 16, 23, 18, ETH_CLOCK_GPIO0_IN);
#elif defined(IOT_NODE_OLIMEX_ETH)
        ETH.begin(ETH_PHY_LAN8720, ETH_PHY_ADDR_AUTO, 12, 23, 18, ETH_CLOCK_GPIO17_OUT);
#else
        ETH.begin();
#endif

#ifdef IOT_NODE_IP_STATIC
        ETH.config(
            state.interfaceConfig.ip,
            state.interfaceConfig.gateway,
            state.interfaceConfig.netmask,
            state.interfaceConfig.gateway);
#endif

        ETH.setHostname(IOT_NODE_NAME);
      }

      void Class::handleConnected()
      {
        state.isConnected = true;

#ifdef IOT_NODE_LOGGING
        Log::debug("event: connect");
#endif

        state.callbacks.connected();
      }

      void Class::handleDhcpTimeout()
      {
#ifdef IOT_NODE_LOGGING
        Log::debug("event: network-config.dhcp-timeout");
#endif

        state.callbacks.dhcpTimeout();
      }

      void Class::handleDisconnected(uint8_t reason)
      {
        state.isConnected = false;

#ifdef IOT_NODE_LOGGING
        Log::debug("event: disconnect");
        Log::debug(fmt::format("event.disconnect.reason: {}", reason));
#endif

        state.callbacks.disconnected();
      }

      void Class::handleGotIP(IPAddress ip, IPAddress gateway, IPAddress netmask)
      {
        state.isConnected = true;

#ifdef IOT_NODE_LOGGING
        Log::debug("event: network-config");
        Log::debug("event.network-config.ip", ip.toString());
        Log::debug("event.network-config.gateway", gateway.toString());
        Log::debug("event.network-config.netmask", netmask.toString());
#endif

        state.callbacks.gotIP(ip);
      }

      bool Class::isConnected()
      {
        return state.isConnected;
      }

      void Class::onBeforeRestart(std::function<void()> callback)
      {
        state.callbacks.beforeRestart = callback;
      }

      void Class::onConnected(std::function<void()> callback)
      {
        state.callbacks.connected = callback;
      }

      void Class::onDhcpTimeout(std::function<void()> callback)
      {
        state.callbacks.dhcpTimeout = callback;
      }

      void Class::onDisconnected(std::function<void()> callback)
      {
        state.callbacks.disconnected = callback;
      }

      void Class::onGotIP(std::function<void(IPAddress ip)> callback)
      {
        state.callbacks.gotIP = callback;
      }

      void Class::onReconnect(std::function<void()> callback)
      {
        state.callbacks.reconnect = callback;
      }

      void Class::update()
      {
        unsigned long now = millis();

        // do not act if ETH is connected
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
            Log::debug("info.first-connect: succeeded");
#endif

            debug(true);
          }

          state.isReconnecting = false;
          state.disconnectionTime = 0;

          unsigned long timeSinceEthMaintenance = now - state.maintenanceTime;

          if (
              now > state.timings.runDebugEvery && timeSinceEthMaintenance > state.timings.runDebugEvery)
          {
            state.maintenanceTime = now;

#ifdef IOT_NODE_LOGGING
            Log::debug(fmt::format("info.maintenance: {}", timeSinceEthMaintenance));
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
            Log::debug("info.first-connect: attempting");
#endif

            state.isReconnecting = true;
            ethConnect();
          }

          return;
        }

        // as soon as ETH becomes disconnected,
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

        unsigned long timeSinceEthDisconnect = now - state.disconnectionTime;

        if (
            !state.isReconnecting && timeSinceEthDisconnect > state.timings.tryReconnectAfter)
        {
          state.isReconnecting = true;

#ifdef IOT_NODE_LOGGING
          Log::debug(fmt::format("info.attempt-reconnect: {}", timeSinceEthDisconnect));
#endif

          ethConnect();

          state.callbacks.reconnect();
        }

        if (timeSinceEthDisconnect > state.timings.restartAfter)
        {
#ifdef IOT_NODE_LOGGING
          Log::debug(fmt::format("info.attempt-restart: {}", timeSinceEthDisconnect));
#endif

          state.callbacks.beforeRestart();

          ESP.restart();

          while (true)
          {
            yield();
          }
        }
      }

      void Class::debug(bool deep)
      {
#ifdef IOT_NODE_LOGGING
        Log::debug(fmt::format("info.eth.link-speed: {}", ETH.linkSpeed()));

#ifdef IOT_NODE_IP_DHCP
        IPAddress ip = ETH.localIP();
        IPAddress gateway = ETH.gatewayIP();
        IPAddress netmask = ETH.subnetMask();

        Log::debug("info.network-config.ip", ip.toString());
        Log::debug("info.network-config.gateway", gateway.toString());
        Log::debug("info.network-config.netmask", netmask.toString());
#endif
#endif
      }
    }

  } // section namespace
} // project namespace

#endif
