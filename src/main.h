#ifndef _MAIN
#define _MAIN

#include <Arduino.h>

#ifdef ARDUINO_ARCH_ESP8266
  #include <ESP8266WiFi.h>
#endif
#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>
#endif

#ifdef IOT_NODE_LINK_ETH
  #include "./persistent-link/eth.h"
#else
  #include "./persistent-link/wifi.h"
#endif

#include "./persistent-link/config.h"
#include "./buttons/config.h"
#include "./buttons/main.h"
#include "./udp-messaging.h"
#include "./logging.h"
#include "./services/hello/service.h"
#include "./services/system-info/service.h"
#include "./services/async/service.h"
#include "./services/relais/service.h"
#include "./services/keepalive/service.h"
#include "./events/button/event.h"

void possiblyDeferredSetup();

void setup();

void loop();

#endif
