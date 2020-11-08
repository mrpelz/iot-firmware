#if defined(IOT_NODE_AP_ALGORE) || defined(IOT_NODE_AP_ELZAR) || defined(IOT_NODE_AP_RICHARDNIXON) || defined(IOT_NODE_AP_SPIROAGNEW)
  #define IOT_NODE_ADVANCED_WIFI_CONFIG
#endif

#define TOSTRING(x) #x
#define STR(x) TOSTRING(x)

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "./persistent-link.h"
#include "./udp-messaging.h"
#include "./services/hello/service.h"
#include "./services/system-info/service.h"
#include "./services/async/service.h"
#include "./services/keepalive/service.h"
#include "./logging.h"

void possiblyDeferredSetup();

void setup();

void loop();
