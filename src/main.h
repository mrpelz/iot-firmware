#ifndef _MAIN
#define _MAIN

#include <Arduino.h>

#include "./events/button/setup.h"
#include "./events/motion-sensor/setup.h"
#include "./services/async/setup.h"
#include "./services/bme280/setup.h"
#include "./services/ccs811/setup.h"
#include "./services/hello/setup.h"
#include "./services/indicator/setup.h"
#include "./services/keepalive/setup.h"
#include "./services/mcp9808/setup.h"
#include "./services/mhz19/setup.h"
#include "./services/relais/setup.h"
#include "./services/sds011/setup.h"
#include "./services/sgp30/setup.h"
#include "./services/system-info/setup.h"
#include "./services/tsl2561/setup.h"
#include "./services/veml6070/setup.h"
#include "./utils/button/setup.h"
#include "./utils/i2c.h"
#include "./utils/indicator/setup.h"
#include "./utils/link/setup.h"
#include "./utils/log.h"
#include "./utils/relais/setup.h"
#include "./utils/udp/setup.h"

namespace IotNode {} // project namespace

#endif
