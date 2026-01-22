#ifdef IOT_NODE_TSL2561

#include "./sensor.h"

namespace IotNode
{
  namespace Services
  {

    namespace Tsl2561
    {
      Sensor::Sensor() : Adafruit_TSL2561_Unified(TSL2561_CHOSEN_ADDRESS, 12345) {}

      float Sensor::calculateLux(uint16_t broadband, uint16_t ir)
      {
        unsigned long chScale;
        unsigned long channel1;
        unsigned long channel0;

        /* Make sure the sensor isn't saturated! */
        uint16_t clipThreshold;
        switch (_tsl2561IntegrationTime)
        {
        case TSL2561_INTEGRATIONTIME_13MS:
          clipThreshold = TSL2561_CLIPPING_13MS;
          break;
        case TSL2561_INTEGRATIONTIME_101MS:
          clipThreshold = TSL2561_CLIPPING_101MS;
          break;
        default:
          clipThreshold = TSL2561_CLIPPING_402MS;
          break;
        }

        /* Return 65536 lux if the sensor is saturated */
        if ((broadband > clipThreshold) || (ir > clipThreshold))
        {
          return 65536;
        }

        /* Get the correct scale depending on the intergration time */
        switch (_tsl2561IntegrationTime)
        {
        case TSL2561_INTEGRATIONTIME_13MS:
          chScale = TSL2561_LUX_CHSCALE_TINT0;
          break;
        case TSL2561_INTEGRATIONTIME_101MS:
          chScale = TSL2561_LUX_CHSCALE_TINT1;
          break;
        default: /* No scaling ... integration time = 402ms */
          chScale = (1 << TSL2561_LUX_CHSCALE);
          break;
        }

        /* Scale for gain (1x or 16x) */
        if (!_tsl2561Gain)
          chScale = chScale << 4;

        /* Scale the channel values */
        channel0 = (broadband * chScale) >> TSL2561_LUX_CHSCALE;
        channel1 = (ir * chScale) >> TSL2561_LUX_CHSCALE;

        /* Find the ratio of the channel values (Channel1/Channel0) */
        unsigned long ratio1 = 0;
        if (channel0 != 0)
          ratio1 = (channel1 << (TSL2561_LUX_RATIOSCALE + 1)) / channel0;

        /* round the ratio value */
        unsigned long ratio = (ratio1 + 1) >> 1;

        unsigned int b, m;

#ifdef TSL2561_PACKAGE_CS
        if ((ratio >= 0) && (ratio <= TSL2561_LUX_K1C))
        {
          b = TSL2561_LUX_B1C;
          m = TSL2561_LUX_M1C;
        }
        else if (ratio <= TSL2561_LUX_K2C)
        {
          b = TSL2561_LUX_B2C;
          m = TSL2561_LUX_M2C;
        }
        else if (ratio <= TSL2561_LUX_K3C)
        {
          b = TSL2561_LUX_B3C;
          m = TSL2561_LUX_M3C;
        }
        else if (ratio <= TSL2561_LUX_K4C)
        {
          b = TSL2561_LUX_B4C;
          m = TSL2561_LUX_M4C;
        }
        else if (ratio <= TSL2561_LUX_K5C)
        {
          b = TSL2561_LUX_B5C;
          m = TSL2561_LUX_M5C;
        }
        else if (ratio <= TSL2561_LUX_K6C)
        {
          b = TSL2561_LUX_B6C;
          m = TSL2561_LUX_M6C;
        }
        else if (ratio <= TSL2561_LUX_K7C)
        {
          b = TSL2561_LUX_B7C;
          m = TSL2561_LUX_M7C;
        }
        else if (ratio > TSL2561_LUX_K8C)
        {
          b = TSL2561_LUX_B8C;
          m = TSL2561_LUX_M8C;
        }
#else
        if ((ratio >= 0) && (ratio <= TSL2561_LUX_K1T))
        {
          b = TSL2561_LUX_B1T;
          m = TSL2561_LUX_M1T;
        }
        else if (ratio <= TSL2561_LUX_K2T)
        {
          b = TSL2561_LUX_B2T;
          m = TSL2561_LUX_M2T;
        }
        else if (ratio <= TSL2561_LUX_K3T)
        {
          b = TSL2561_LUX_B3T;
          m = TSL2561_LUX_M3T;
        }
        else if (ratio <= TSL2561_LUX_K4T)
        {
          b = TSL2561_LUX_B4T;
          m = TSL2561_LUX_M4T;
        }
        else if (ratio <= TSL2561_LUX_K5T)
        {
          b = TSL2561_LUX_B5T;
          m = TSL2561_LUX_M5T;
        }
        else if (ratio <= TSL2561_LUX_K6T)
        {
          b = TSL2561_LUX_B6T;
          m = TSL2561_LUX_M6T;
        }
        else if (ratio <= TSL2561_LUX_K7T)
        {
          b = TSL2561_LUX_B7T;
          m = TSL2561_LUX_M7T;
        }
        else if (ratio > TSL2561_LUX_K8T)
        {
          b = TSL2561_LUX_B8T;
          m = TSL2561_LUX_M8T;
        }
#endif

        unsigned long temp;
        temp = ((channel0 * b) - (channel1 * m));

        /* Do not allow negative lux value */
        if (temp < 0)
          temp = 0;

        float lux = (float)temp / (1 << TSL2561_LUX_LUXSCALE);
        return lux;
      }
    }

  } // section namespace
} // project namespace

#endif
