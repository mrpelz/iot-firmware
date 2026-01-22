#pragma once

#ifdef IOT_NODE_TSL2561

#include <Arduino.h>
#include <Wire.h>

#define private protected
#include <Adafruit_TSL2561_U.h>
#undef private

#ifdef IOT_NODE_BOARD_ROOM_SENSOR
#define TSL2561_CHOSEN_ADDRESS TSL2561_ADDR_LOW
#else
#define TSL2561_CHOSEN_ADDRESS TSL2561_ADDR_FLOAT
#endif

namespace IotNode
{
  namespace Services
  {

    namespace Tsl2561
    {
      class Sensor : public Adafruit_TSL2561_Unified
      {
      public:
        Sensor();
        float calculateLux(uint16_t broadband, uint16_t ir);
      };
    }

  } // section namespace
} // project namespace

#endif
