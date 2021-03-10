#ifndef _SERVICES_CCS811_MAIN
#define _SERVICES_CCS811_MAIN

#ifdef IOT_NODE_CCS811

#include <Arduino.h>
#include <Wire.h>

#include "../../utils/log.h"
#include "../../utils/udp/main.h"
#include "./sensor.h"

namespace IotNode {
namespace Services {

namespace Ccs811 {
  void initializer(TwoWire *i2c);

  void responseTask(void *parameter);

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond);
}

} // section namespace
} // project namespace

#endif

#endif
