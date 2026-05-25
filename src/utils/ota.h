#pragma once

#include <Arduino.h>

#include <ArduinoOTA.h>

#include "./log.h"

namespace IotNode::Utils::OTA
{
  void update();
  void setup();
}
