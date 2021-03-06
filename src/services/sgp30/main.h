#ifndef _SGP30_MAIN
#define _SGP30_MAIN

#ifdef IOT_NODE_SGP30

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_SGP30.h>

#include "../../utils/log.h"
#include "../../utils/udp/main.h"

namespace IotNode {

namespace Sgp30 {
  void initializer(TwoWire *i2c);

  void responseTask(void *parameter);

  void handler(UDP::Payload *request, UDP::RespondCallback respond);
}

} // project namespace

#endif

#endif
