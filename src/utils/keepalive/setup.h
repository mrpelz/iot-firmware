#pragma once

#include <Arduino.h>

#include "../udp/setup.h"
#include "./main.h"

#ifdef IOT_NODE_POWER_CYCLE_PROTECTION
#if defined(ENV_SCOPED_SETUP) && defined(IOT_NODE_BOARD_SHELLY1)
#include "../../setup/Shelly1/output.h"
#elif defined(ENV_SCOPED_SETUP) && (defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V1) || defined(IOT_NODE_BOARD_SONOFF_BASIC_R2V13))
#include "../../setup/Sonoff_Basic_R2/output.h"
#else
#include "../output/setup.h"
#endif
#endif

namespace IotNode::Utils::Keepalive
{
  extern Class keepalive;
  extern Class eventPeer;

#ifdef IOT_NODE_POWER_CYCLE_PROTECTION
  extern Class powerCycleProtection;
#endif

  void update();

#ifdef IOT_NODE_ESP32
  void task(void *parameter);
#endif

  void setup();
}
