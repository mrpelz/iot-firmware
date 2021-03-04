#ifndef _TSL2561_SENSOR
#define _TSL2561_SENSOR

#ifdef IOT_NODE_TSL2561

#include <Arduino.h>
#include <Wire.h>

#include <SparkFunTSL2561.h>

#define TSL2561_CHOSEN_ADDR TSL2561_ADDR

namespace IotNode {

namespace Tsl2561 {
  class Sensor : public SFE_TSL2561 {
    private:
      TwoWire *_i2c;
    public:
      void begin(TwoWire *i2c);
      boolean clearInterrupt(void);
      boolean readByte(unsigned char address, unsigned char &value);
      boolean writeByte(unsigned char address, unsigned char value);
      boolean readUInt(unsigned char address, unsigned int &value);
  };
}

} // project namespace

#endif

#endif
