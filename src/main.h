#if defined(IOT_NODE_AP_ALGORE) || defined(IOT_NODE_AP_ELZAR) || defined(IOT_NODE_AP_RICHARDNIXON) || defined(IOT_NODE_AP_SPIROAGNEW)
  #define IOT_NODE_ADVANCED_WIFI_CONFIG
#endif

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "./persistent-link.h"
#include "./udp-messaging.h"
#include "./services/test/service.h"
#include "./logging.h"

void possiblyDeferredSetup();

void setup();

void loop();
