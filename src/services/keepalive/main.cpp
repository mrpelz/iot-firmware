#include "./main.h"

namespace IotNode
{
  namespace Services
  {

    namespace Keepalive
    {
      Utils::UDP::RequestHandler makeHandler()
      {
        auto handler = [](
                           Utils::UDP::Payload *request,
                           Utils::UDP::RespondCallback respond,
                           Utils::UDP::Peer peer)
        {
#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("keepalive-service", "got request");
#endif

          auto restart = request->size() >= 1 && request->at(0) != 0;
          if (restart)
          {
#ifdef IOT_NODE_LOGGING
            Utils::Log::debug("keepalive-service", "force restart");
#endif

            ESP.restart();
          }

          Utils::Keepalive::keepalive.tick();
          Utils::Keepalive::eventPeer.tick();

#ifdef IOT_NODE_POWER_CYCLE_PROTECTION
          Utils::Keepalive::powerCycleProtection.stop();
#endif

          Utils::UDP::instance.setEventPeer(peer);

#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("keepalive-service", "sending response");
#endif

          respond({});
        };

        return handler;
      }
    }

  } // section namespace
} // project namespace
