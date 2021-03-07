#ifndef _VEML6070_MAIN
#define _VEML6070_MAIN

#ifdef IOT_NODE_VEML6070

#include <Arduino.h>
#include <Wire.h>

#include "../../utils/log.h"
#include "../../utils/udp/main.h"
#include "./sensor.h"

namespace IotNode {

namespace Veml6070 {
  void initializer(TwoWire *i2c);

  void responseTask(void *parameter);

  void handler(UDP::Payload *request, UDP::RespondCallback respond);
}

} // project namespace

#endif

#endif
