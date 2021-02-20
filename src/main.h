#ifndef _MAIN
#define _MAIN

#include <Arduino.h>

#include <Wire.h>
#include <SPI.h>

#include "./events/button/config.h"
#include "./events/button/event.h"
#include "./events/button/handler.h"
#include "./services/async/service.h"
#include "./services/bme280/service.h"
#include "./services/hello/service.h"
#include "./services/keepalive/service.h"
#include "./services/mcp9808/service.h"
#include "./services/relais/service.h"
#include "./services/system-info/service.h"
#include "./services/tsl2561/handler.h"
#include "./services/tsl2561/service.h"
#include "./utils/logging.h"
#include "./utils/link/setup.h"
#include "./utils/udp-messaging.h"

void possiblyDeferredSetup();

void setup();

void loop();

#endif
