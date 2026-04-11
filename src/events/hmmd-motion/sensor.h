#pragma once

#ifdef IOT_NODE_HMMD_MOTION

#include <Arduino.h>

#define private protected
#include <s3km1110.h>
#undef private

namespace IotNode
{
  namespace Events
  {

    namespace HmmdMotion
    {
      class Sensor : public s3km1110
      {
      public:
        bool begin(Stream &dataStream);
      };
    }

  } // section namespace
} // project namespace

#endif
