#include "./main.h"

#ifdef IOT_NODE_INDICATORS

namespace IotNode
{
  namespace Services
  {

    namespace Indicator
    {
      Utils::UDP::Service makeService(Utils::Indicator::ClassPin *indicator, uint8_t index)
      {
        auto handler = [indicator, index](
                           Utils::UDP::Payload *request,
                           Utils::UDP::RespondCallback respond,
                           Utils::UDP::Peer peer)
        {
#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("indicator-service: got request");
#endif

          if (request->size() < 1)
          {
#ifdef IOT_NODE_LOGGING
            Utils::Log::debug("indicator-service: missing parameters");
#endif

            return;
          }

          auto cmd = request->at(0);

          switch ((uint8_t)cmd)
          {
          case INDICATOR_CMD_ON:
            indicator->setOn(true);
            break;
          case INDICATOR_CMD_OFF:
            indicator->setOn(false);
            break;
          case INDICATOR_CMD_BLINK:
            if (request->size() >= 2)
            {
              auto count = request->at(1);

              indicator->blink((uint8_t)count);
            }
            else
            {
              indicator->blink();
            }
          }

#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("indicator-service: sending response");
#endif

          respond({});
        };

        return {
            ids::indicator,
            index,
            handler};
      }

#ifdef IOT_NODE_SX1509
      Utils::UDP::Service makeService(Utils::Indicator::ClassExpander *indicator, uint8_t index)
      {
        auto handler = [indicator, index](
                           Utils::UDP::Payload *request,
                           Utils::UDP::RespondCallback respond,
                           Utils::UDP::Peer peer)
        {
#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("indicator-service: got request");
#endif

          if (request->size() < 1)
          {
#ifdef IOT_NODE_LOGGING
            Utils::Log::debug("indicator-service: missing parameters");
#endif

            return;
          }

          auto cmd = request->at(0);

          switch ((uint8_t)cmd)
          {
          case INDICATOR_CMD_ON:
            indicator->setOn(true);
            break;
          case INDICATOR_CMD_OFF:
            indicator->setOn(false);
            break;
          case INDICATOR_CMD_BLINK:
            if (request->size() >= 2)
            {
              auto count = request->at(1);

              indicator->blink((uint8_t)count);
            }
            else
            {
              indicator->blink();
            }
          }

#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("indicator-service: sending response");
#endif

          respond({});
        };

        return {
            ids::indicator,
            index,
            handler};
      }
#endif
    }

  } // section namespace
} // project namespace

#endif
