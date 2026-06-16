#pragma once

#ifdef IOT_NODE_ENV_SONOFF_BASIC_R2_V1_MRPELZBEDROOM_NIGHTLIGHT

#include <Arduino.h>

#include <pre.h>
#include <post.h>

#include "../../utils/keepalive/setup.h"

#ifdef IOT_NODE_LOGGING
#include "../../utils/log.h"
#endif

#include "../../utils/udp/setup.h"
#include "../../utils/link/setup.h"

#include "../../services/hello/setup.h"
#include "../../services/keepalive/setup.h"
#include "../../services/system-info/setup.h"

#include "../Sonoff_Basic_R2/button.h"
#include "../Sonoff_Basic_R2/indicator.h"
#include "../Sonoff_Basic_R2/output.h"

#include "../../utils/ota.h"

namespace IotNode::Setup
{
  void setup();

  void loop();
}

#endif
