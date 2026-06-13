#pragma once

#include <Arduino.h>

#include <pre.h>
#include <post.h>
#include <functional>

#include "./services/hello/setup.h"
#include "./services/keepalive/setup.h"
#include "./services/system-info/setup.h"
#include "./utils/keepalive/setup.h"
#include "./utils/link/setup.h"
#include "./utils/udp/setup.h"

#ifndef IOT_NODE_NO_OTA
#include "./utils/ota.h"
#endif

#ifdef IOT_NODE_SX1509
#include "./utils/sx1509/setup.h"
#endif

#ifdef IOT_NODE_BUTTONS
#include "./events/button/setup.h"
#endif

#ifdef IOT_NODE_ESP_NOW_GW
#include "./events/esp-now-gw/setup.h"
#endif

#ifdef IOT_NODE_INPUT
#include "./events/input/setup.h"
#endif

#ifdef IOT_NODE_HMMD_MOTION
#include "./events/hmmd-motion/setup.h"
#endif

#ifdef IOT_NODE_RF_433
#include "./events/rf-433/setup.h"
#endif

#ifdef IOT_NODE_VCC
#include "./events/vcc/setup.h"
#endif

#ifdef IOT_NODE_ASYNC
#include "./services/async/setup.h"
#endif

#ifdef IOT_NODE_BME280
#include "./services/bme280/setup.h"
#endif

#ifdef IOT_NODE_CCS811
#include "./services/ccs811/setup.h"
#endif

#ifdef IOT_NODE_INDICATORS
#include "./services/indicator/setup.h"
#include "./services/output-ng/setup.h"
#endif

#ifdef IOT_NODE_LED
#include "./services/led/setup.h"
#endif

#ifdef IOT_NODE_MCP9808
#include "./services/mcp9808/setup.h"
#endif

#ifdef IOT_NODE_MHZ19
#include "./services/mhz19/setup.h"
#endif

#ifdef IOT_NODE_OUTPUT
#include "./services/output/setup.h"
#endif

#ifdef IOT_NODE_SDS011
#include "./services/sds011/setup.h"
#endif

#ifdef IOT_NODE_SGP30
#include "./services/sgp30/setup.h"
#endif

#ifdef IOT_NODE_TSL2561
#include "./services/tsl2561/setup.h"
#endif

#ifdef IOT_NODE_VEML6070
#include "./services/veml6070/setup.h"
#endif

#ifdef IOT_NODE_BUTTONS
#include "./utils/button/setup.h"
#endif

#ifdef IOT_NODE_E_PAPER
#include "./utils/e-paper/setup.h"
#endif

#ifdef IOT_NODE_EQ3
#include "./utils/eq3/setup.h"
#endif

#ifdef IOT_NODE_ESP_NOW_NODE
#include "./utils/esp-now-node.h"
#endif

#ifdef IOT_NODE_I2C
#include "./utils/i2c.h"
#endif

#ifdef IOT_NODE_INDICATORS
#include "./utils/indicator/setup.h"
#include "./utils/output-ng/setup.h"
#endif

#ifdef IOT_NODE_LED
#include "./utils/led/setup.h"
#endif

#ifdef IOT_NODE_LOGGING
#include "./utils/log.h"
#endif

#ifdef IOT_NODE_OUTPUT
#include "./utils/output/setup.h"
#endif

#ifdef IOT_NODE_VCC
#include "./utils/vcc.h"
#endif
