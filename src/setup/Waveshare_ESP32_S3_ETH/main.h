#pragma once

#ifdef IOT_NODE_ENV_WAVESHARE_ESP32_S3_ETH

#include <Arduino.h>

#include <pre.h>
#include <post.h>

#include "../../utils/keepalive/setup.h"

#ifdef IOT_NODE_LOGGING
#include "../../utils/log.h"
#endif

#include "../../utils/link/setup.h"
#include "../../utils/udp/setup.h"

#include "../../services/hello/setup.h"
#include "../../services/keepalive/setup.h"
#include "../../services/system-info/setup.h"

#include "./output-ng.h"

#include "../../utils/ota.h"

namespace IotNode::Setup
{
  void setup();
  void loop();
}

#endif
