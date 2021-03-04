#ifndef _TSL2561_SENSOR
#define _TSL2561_SENSOR

#ifdef IOT_NODE_TSL2561

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>

#define private protected
#include <Adafruit_TSL2561_U.h>
#undef private

#define TSL2561_CHOSEN_ADDRESS TSL2561_ADDR_FLOAT

namespace IotNode {

namespace Tsl2561 {
  class Sensor : public Adafruit_TSL2561_Unified {
    public:
      Sensor();
      float calculateLux(uint16_t broadband, uint16_t ir);
  };
}

} // project namespace

#endif

#endif
