#ifdef IOT_NODE_SX1509

#include "./setup.h"

#define CLOCK_DIV 4

namespace IotNode
{
  namespace Utils
  {

    namespace Sx1509
    {
      bool working = false;

      SX1509 io;

      void setup()
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("sx1509-service: initializing io");
#endif

        working = io.begin(SX1509_ADDRESS, I2C::bus);

        if (!working)
        {
#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("sx1509-service: io initialization failed");
#endif

          return;
        }
      }
    }

  } // section namespace
} // project namespace

#endif
