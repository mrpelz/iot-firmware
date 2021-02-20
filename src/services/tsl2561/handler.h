#ifndef _TSL2561_HANDLER
#define _TSL2561_HANDLER

#include <Arduino.h>
#include <Wire.h>

#include <SparkFunTSL2561.h>

#include "../../utils/udp.h"
#include "../../utils/log.h"

namespace IotNode {

struct UpdateHandler {
  RequestHandler handler;
  std::function<void ()> update;
};

UpdateHandler makeTsl2561UpdateHandler();

} // project namespace

#endif
