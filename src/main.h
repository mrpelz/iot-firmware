#ifndef _MAIN
#define _MAIN

#include <Arduino.h>
#include <functional>

#include "./events/button/setup.h"
#include "./events/esp-now-gw/setup.h"
#include "./events/input/setup.h"
#include "./events/rf-433/setup.h"
#include "./events/vcc/setup.h"
#include "./services/async/setup.h"
#include "./services/e-paper/setup.h"
#include "./services/bme280/setup.h"
#include "./services/ccs811/setup.h"
#include "./services/hello/setup.h"
#include "./services/indicator/setup.h"
#include "./services/keepalive/setup.h"
#include "./services/led/setup.h"
#include "./services/mcp9808/setup.h"
#include "./services/mhz19/setup.h"
#include "./services/output/setup.h"
#include "./services/sds011/setup.h"
#include "./services/sgp30/setup.h"
#include "./services/system-info/setup.h"
#include "./services/tsl2561/setup.h"
#include "./services/veml6070/setup.h"
#include "./utils/button/setup.h"
#include "./utils/e-paper/setup.h"
#include "./utils/esp-now-node.h"
#include "./utils/i2c.h"
#include "./utils/indicator/setup.h"
#include "./utils/keepalive/setup.h"
#include "./utils/led/setup.h"
#include "./utils/link/setup.h"
#include "./utils/log.h"
#include "./utils/ota.h"
#include "./utils/output/setup.h"
#include "./utils/udp/setup.h"
#include "./utils/vcc.h"

namespace IotNode {} // project namespace

#endif
