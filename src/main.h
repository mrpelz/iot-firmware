#ifndef _MAIN
#define _MAIN

#include <Arduino.h>

#include <Wire.h>
#include <SPI.h>

#include "./events/button/setup.h"
#include "./services/async/handler.h"
#include "./services/async/service.h"
#include "./services/bme280/service.h"
#include "./services/hello/service.h"
#include "./services/keepalive/handler.h"
#include "./services/keepalive/service.h"
#include "./services/mcp9808/service.h"
#include "./services/relais/setup.h"
#include "./services/system-info/service.h"
#include "./services/tsl2561/service.h"
#include "./utils/link/setup.h"
#include "./utils/log.h"
#include "./utils/udp/setup.h"

namespace IotNode {} // project namespace

#endif
