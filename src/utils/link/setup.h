#pragma once

#include <Arduino.h>

#include <ArduinoOTA.h>

#include "../udp/setup.h"
#include "./main.h"

#ifdef IOT_NODE_INDICATORS
#if defined(ENV_SCOPED_SETUP) && defined(IOT_NODE_BOARD_TEST_DEVICE)
#include "../../setup/Test_Device/indicator.h"
#elif defined(ENV_SCOPED_SETUP) && defined(IOT_NODE_BOARD_ROOM_SENSOR)
#include "../../setup/Test_mrpelzBedroom_roomSensor/indicator.h"
#elif defined(ENV_SCOPED_SETUP) && defined(IOT_NODE_BOARD_OBI_JACK)
#include "../../setup/Obi_Jack/indicator.h"
#elif defined(ENV_SCOPED_SETUP) && defined(IOT_NODE_BOARD_H801)
#include "../../setup/H801/indicator.h"
#elif defined(ENV_SCOPED_SETUP) && (defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V1) || defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V13))
#include "../../setup/Sonoff_Basic_R2/indicator.h"
#elif defined(ENV_SCOPED_SETUP) && defined(IOT_NODE_BOARD_SHELLYPLUS1)
#include "../../setup/Shelly_Plus_1/indicator.h"
#endif
#endif

#ifdef IOT_NODE_ESP8266
#include <ESP8266mDNS.h>
#endif
#ifdef IOT_NODE_ESP32
#include <ESPmDNS.h>
#endif

#ifdef IOT_NODE_LINK_ETH
#include "./eth.h"
#else
#include "./wifi.h"
#endif

namespace IotNode::Utils::Link
{
  static const Timings timings = {
      .runDebugEvery = 60000,
      .tryReconnectAfter = 2000,
      .restartAfter = 20000};

#ifdef IOT_NODE_LINK_WIFI
  static const Credentials credentials = {
      .ssid = IOT_NODE_WIFI_SSID,
      .password = IOT_NODE_WIFI_PSK,

#ifdef IOT_NODE_ADVANCED_WIFI_CONFIG
      .bssid = IOT_NODE_WIFI_BSSID,
      .channel = IOT_NODE_WIFI_CHANNEL,
#endif
  };

#ifdef IOT_NODE_ESP8266
#include <ESP8266WiFi.h>

  static const PhyMode_t phyMode = WIFI_PHY_MODE_11N;

  static const OutputPower_t outputPower = 6;
#endif

#ifdef IOT_NODE_ESP32
#include <WiFi.h>

  static const PhyMode_t phyMode = WIFI_PHY_RATE_54M;

  static const OutputPower_t outputPower = WIFI_POWER_7dBm;
#endif
#endif

#ifdef IOT_NODE_IP_STATIC
  static const InterfaceConfig interfaceConfig = {
      .ip = IPAddress(IOT_NODE_IP),
      .gateway = IPAddress(IOT_NODE_GATEWAY),
      .netmask = IPAddress(IOT_NODE_NETMASK),
  };
#endif

#ifdef IOT_NODE_LINK_ETH
#ifdef IOT_NODE_ESP32
  static const Config config = {
#ifdef IOT_NODE_IP_STATIC
      .interfaceConfig = interfaceConfig,
#endif

      .timings = timings};
#endif
#else
  static const Config config = {
      .phyMode = phyMode,
      .outputPower = outputPower,

#ifdef IOT_NODE_IP_STATIC
      .interfaceConfig = interfaceConfig,
#endif

#ifdef IOT_NODE_LINK_WIFI
      .credentials = credentials,
#endif

      .timings = timings};
#endif

  extern Class link;

  void update();

#ifdef IOT_NODE_ESP32
  void task(void *parameter);
#endif

  void setup();
}
