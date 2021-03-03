#ifndef _TSL2561_MAIN
#define _TSL2561_MAIN

#ifdef IOT_NODE_TSL2561

#include <Arduino.h>
#include <Wire.h>

#include <SparkFunTSL2561.h>

#include "../../utils/log.h"
#include "../../utils/udp/main.h"

namespace IotNode {

namespace Tsl2561 {
  void initializer(TwoWire *i2c);

  void responseTask(void * parameter);

  void handler(UDP::Payload *request, UDP::RespondCallback respond);
}

} // project namespace

#endif

#endif
