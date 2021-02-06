#ifndef _MAIN
#define _MAIN

#include <Arduino.h>

#include <Wire.h>
#include <SPI.h>

#ifdef ARDUINO_ARCH_ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>
#endif

#ifdef IOT_NODE_LINK_ETH
  #include "./utils/persistent-link/eth.h"
#else
  #include "./utils/persistent-link/wifi.h"
#endif

#include "./utils/logging.h"
#include "./utils/persistent-link/config.h"
#include "./utils/udp-messaging.h"
#include "./events/button/config.h"
#include "./events/button/event.h"
#include "./events/button/handler.h"
#include "./services/async/service.h"
#include "./services/hello/service.h"
#include "./services/keepalive/service.h"
#include "./services/mcp9808/service.h"
#include "./services/relais/service.h"
#include "./services/system-info/service.h"

void possiblyDeferredSetup();

void setup();

void loop();

#endif
