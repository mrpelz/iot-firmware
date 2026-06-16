#pragma once

#ifdef IOT_NODE_BOARD_ROOM_SENSOR

#include <Arduino.h>

#include <pre.h>
#include <post.h>

#include "../../utils/keepalive/setup.h"

#ifdef IOT_NODE_LOGGING
#include "../../utils/log.h"
#endif

#include "../../utils/i2c.h"
#include "../../utils/sx1509/setup.h"
#include "../../utils/udp/setup.h"
#include "../../utils/link/setup.h"

#include "../../services/hello/setup.h"
#include "../../services/keepalive/setup.h"
#include "../../services/system-info/setup.h"
#include "../../services/bme280/setup.h"
#include "../../services/mcp9808/setup.h"
#include "../../services/sgp30/setup.h"
#include "../../services/tsl2561/setup.h"

#include "./indicator.h"

#include "../../utils/ota.h"

namespace IotNode::Setup
{
  void setup();

  void loop();
}

#endif
