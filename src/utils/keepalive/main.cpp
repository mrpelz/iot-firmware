#include "./main.h"

namespace IotNode
{
  namespace Utils
  {

    namespace Keepalive
    {
      Class::Class(unsigned long timeout, KeepaliveCallback callback)
      {
        state.timeout = timeout;
        state.callback = callback;
      }

      void Class::start()
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("keepalive: start");
#endif

        state.running = true;
      }

      void Class::stop()
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("keepalive: stop");
#endif

        state.running = false;
      }

      void Class::tick()
      {
        if (!state.running)
        {
          start();
        }

        state.ticked = true;
      }

      void Class::update()
      {
        unsigned long now = millis();

        if (state.ticked)
        {
          state.ticked = false;

#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("keepalive: tick");
#endif

          state.lastTick = now;
        }

        if (!state.running)
          return;

        unsigned long timeSinceTick = now - state.lastTick;

        if (timeSinceTick > state.timeout)
        {
#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("keepalive: trip");
#endif

          state.callback();

          stop();
        }
      }
    }

  } // section namespace
} // project namespace
