#ifndef _SERVICES_SGP30_MAIN
#define _SERVICES_SGP30_MAIN

#ifdef IOT_NODE_SGP30

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_SGP30.h>

#include "../../utils/i2c.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"

namespace IotNode {
namespace Services {

namespace Sgp30 {
  uint32_t getAbsoluteHumidity(float temperature, float humidity);

  void initializer(TwoWire *i2c);

  void responseTask(void *parameter);

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond);
}

} // section namespace
} // project namespace

#endif

#endif
