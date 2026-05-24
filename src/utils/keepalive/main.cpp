#include "./main.h"

namespace IotNode
{
  namespace Utils
  {

    namespace Keepalive
    {
      Class::Class(unsigned long timeout, KeepaliveCallback callback)
      {
        _state.timeout = timeout;
        _state.callback = callback;
      }

      void Class::start()
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("keepalive: start");
#endif

        _state.running = true;
      }

      void Class::stop()
      {
#ifdef IOT_NODE_LOGGING
        Utils::Log::debug("keepalive: stop");
#endif

        _state.running = false;
      }

      void Class::tick()
      {
        if (!_state.running)
        {
          start();
        }

        _state.ticked = true;
      }

      void Class::update()
      {
        unsigned long now = millis();

        if (_state.ticked)
        {
          _state.ticked = false;

#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("keepalive: tick");
#endif

          _state.lastTick = now;
        }

        if (!_state.running)
          return;

        unsigned long timeSinceTick = now - _state.lastTick;

        if (timeSinceTick > _state.timeout)
        {
#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("keepalive: trip");
#endif

          _state.callback();

          stop();
        }
      }
    }

  } // section namespace
} // project namespace
