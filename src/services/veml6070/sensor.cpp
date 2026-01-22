#ifdef IOT_NODE_VEML6070

#include "./sensor.h"

namespace IotNode
{
  namespace Services
  {

    namespace Veml6070
    {
      bool Sensor::begin(TwoWire *i2c)
      {
        i2c->beginTransmission(VEML6070_ADDR_L);

        if (i2c->endTransmission() == 0)
        {
          Adafruit_VEML6070::begin(VEML6070_4_T, i2c);
          return true;
        }

        return false;
      }
    }

  } // section namespace
} // project namespace

#endif
