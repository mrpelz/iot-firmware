#ifdef IOT_NODE_HMMD_MOTION

#include "./sensor.h"

namespace IotNode
{
  namespace Events
  {

    namespace HmmdMotion
    {
      bool Sensor::begin(Stream &dataStream)
      {
        // UARTs
        _uartRadar = &dataStream;

        if (!_uartRadar)
        {
          return false;
        }

#if !defined(S3KM1110_SKIP_READ_CONFIG_ON_BEGIN)
        if (_enableReportMode())
        {
          readAllRadarConfigs();
          return true;
        }
#else
        return _enableReportMode();
#endif // S3KM1110_SKIP_READ_CONFIG_ON_BEGIN

        return false;
      }

    }

  } // section namespace
} // project namespace

#endif
