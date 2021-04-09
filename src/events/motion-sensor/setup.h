#ifndef _EVENTS_MOTION_SENSOR_SETUP
#define _EVENTS_MOTION_SENSOR_SETUP

#ifdef IOT_NODE_MOTION_SENSOR

#include <Arduino.h>

#include "../../utils/udp/setup.h"
#include "./main.h"

namespace IotNode {
namespace Events {

namespace MotionSensor {
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
