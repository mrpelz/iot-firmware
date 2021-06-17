#ifndef _EVENTS_RF_433_SETUP
#define _EVENTS_RF_433_SETUP

#ifdef IOT_NODE_RF_433

#include <Arduino.h>

#include "../../utils/log.h"
#include "./main.h"

namespace IotNode {
namespace Events {

namespace Rf433 {
  void update();

  #ifdef IOT_NODE_ESP32
    void task(void *parameter);
  #endif

  void setup();
}

} // section namespace
} // project namespace

#endif

#endif
