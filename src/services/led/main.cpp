#include "./main.h"

#ifdef IOT_NODE_LED

namespace IotNode
{
  namespace Services
  {

    namespace Led
    {
      Utils::UDP::Service makeService(FadeLed *led, uint8_t index)
      {
        auto handler = [led, index](
                           Utils::UDP::Payload *request,
                           Utils::UDP::RespondCallback respond,
                           Utils::UDP::Peer peer)
        {
#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("led-service", "got request");
          Utils::Log::debug("led-service.index", String(index));
#endif

          auto duty = request->size() >= 1 ? request->at(0) : (uint8_t)0;

#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("led-service.duty", String(duty));
#endif

          led->set(duty);

#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("led-service", "sending response");
#endif

          respond({});
        };

        return {
            ids::led,
            index,
            handler};
      }
    }

  } // section namespace
} // project namespace

#endif
