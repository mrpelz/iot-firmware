#ifndef _VEML6070_SENSOR
#define _VEML6070_SENSOR

#ifdef IOT_NODE_VEML6070

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>

#define private protected
#include <Adafruit_VEML6070.h>
#undef private

namespace IotNode {

namespace Veml6070 {
  class Sensor : public Adafruit_VEML6070 {
    public:
      bool begin(TwoWire *i2c);
  };
}

} // project namespace

#endif

#endif
