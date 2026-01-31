#pragma once

#include <Arduino.h>

#include <ArduinoOTA.h>

#include "./log.h"

namespace IotNode
{
  namespace Utils
  {

    namespace OTA
    {
      void update();
      void setup();
    }

  } // section namespace
} // project namespace
