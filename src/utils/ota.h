#pragma once

#include <Arduino.h>

#include <ArduinoOTA.h>

#include "./log.h"

namespace IotNode {
namespace Utils {

namespace OTA {
  void update();

  #ifdef IOT_NODE_ESP32
    void task(void *parameter);
  #endif

  void setup();
}

} // section namespace
} // project namespace
