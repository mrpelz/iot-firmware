#pragma once

#ifdef IOT_NODE_ENV_OBI_JACK_LIVINGROOM_STANDINGLAMP

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

#include "../Obi_Jack/button.h"
#include "../Obi_Jack/indicator.h"
#include "../Obi_Jack/output.h"

#include "../../utils/ota.h"

namespace IotNode::Setup
{
  void setup();

  void loop();
}

#endif
