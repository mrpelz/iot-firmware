#pragma once

#ifdef IOT_NODE_SGP30

#include <Arduino.h>

#include <pre.h>
#include <post.h>
#include <Wire.h>

#include <Adafruit_SGP30.h>

#include "../../utils/i2c.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"

namespace IotNode::Services::Sgp30
{
  unsigned long getAbsoluteHumidity(float temperature, float humidity);

  void initializer(TwoWire *i2c);

  void responseTask(void *parameter);

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer);
}

#endif
