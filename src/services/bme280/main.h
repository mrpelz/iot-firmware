#pragma once

#ifdef IOT_NODE_BME280

#include <Arduino.h>

#include <pre.h>
#include <post.h>
#include <Wire.h>

#include <Adafruit_BME280.h>

#include "../../utils/i2c.h"
#include "../../utils/log.h"
#include "../../utils/udp/main.h"

namespace IotNode::Services::Bme280
{
  void initializer(TwoWire *i2c);

  void responseTask(void *parameter);

  void handler(Utils::UDP::Payload *request, Utils::UDP::RespondCallback respond, Utils::UDP::Peer peer);
}

#endif
