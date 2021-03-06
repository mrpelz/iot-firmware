#ifndef _CCS811_MAIN
#define _CCS811_MAIN

#ifdef IOT_NODE_CCS811

#include <Arduino.h>
#include <Wire.h>

#include "../../utils/log.h"
#include "../../utils/udp/main.h"
#include "./sensor.h"

namespace IotNode {

namespace Ccs811 {
  void initializer(TwoWire *i2c);

  void responseTask(void *parameter);

  void handler(UDP::Payload *request, UDP::RespondCallback respond);
}

} // project namespace

#endif

#endif
