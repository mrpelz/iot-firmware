#ifndef _MAIN
#define _MAIN

#include <Arduino.h>

#include <Wire.h>
#include <SPI.h>

#include "./events/button/setup.h"
#include "./services/async/setup.h"
#include "./services/bme280/service.h"
#include "./services/hello/setup.h"
#include "./services/keepalive/setup.h"
#include "./services/mcp9808/setup.h"
#include "./services/relais/setup.h"
#include "./services/system-info/setup.h"
#include "./services/tsl2561/service.h"
#include "./utils/link/setup.h"
#include "./utils/log.h"
#include "./utils/udp/setup.h"

namespace IotNode {} // project namespace

#endif
