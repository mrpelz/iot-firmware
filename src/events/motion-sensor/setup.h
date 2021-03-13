#ifndef _EVENTS_MOTION_SENSOR_SETUP
#define _EVENTS_MOTION_SENSOR_SETUP

#include <Arduino.h>

#include "../../utils/log.h"
#include "../../utils/udp/main.h"
#include "./main.h"

namespace IotNode {
namespace Events {

namespace MotionSensor {
  void update();

  #ifdef ARDUINO_ARCH_ESP32
    void task(void *parameter);
  #endif

  void setup();

  void setup(Utils::UDP::Class *udp);
}

} // section namespace
} // project namespace

#endif
