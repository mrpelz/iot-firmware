#ifndef _CCS811_SENSOR
#define _CCS811_SENSOR

#ifdef IOT_NODE_CCS811

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>

#define private protected
#include <Adafruit_CCS811.h>
#undef private

namespace IotNode {

namespace Ccs811 {
  class Sensor : public Adafruit_CCS811 {
    private:
      TwoWire *_i2c;
      void _i2c_init();
      void read(uint8_t reg, uint8_t *buf, uint8_t num);
      void write(uint8_t reg, uint8_t *buf, uint8_t num);
    public:
      bool begin(TwoWire *i2c);
  };
}

} // project namespace

#endif

#endif
