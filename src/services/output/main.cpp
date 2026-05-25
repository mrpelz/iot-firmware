#include "./main.h"

#ifdef IOT_NODE_OUTPUT

namespace IotNode::Services::Output
{
  Utils::UDP::Service makeService(Utils::Output::Regular *output, unsigned char index)
  {
    auto handler = [output, index](
                       Utils::UDP::Payload *request,
                       Utils::UDP::RespondCallback respond,
                       Utils::UDP::Peer peer)
    {
#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("output-service: got request");
      Utils::Log::debug(fmt::format("output-service.index: {}", index));
#endif

      bool on = request->size() >= 1 && request->at(0) != 0;

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug(fmt::format("output-service.on: {}", on));
#endif

      output->setOn(on);

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("output-service: sending response");
#endif

      respond({});
    };

    return {
        ids::output,
        index,
        handler};
  }

  Utils::UDP::Service makeService(Utils::Output::Pulse *output, unsigned char index)
  {
    auto handler = [output, index](
                       Utils::UDP::Payload *request,
                       Utils::UDP::RespondCallback respond,
                       Utils::UDP::Peer peer)
    {
#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("output-service: got request");
      Utils::Log::debug(fmt::format("output-service.index: {}", index));
#endif

      bool on = request->size() >= 1 && request->at(0) != 0;

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug(fmt::format("output-service.on: {}", on));
#endif

      output->setOn(on);

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("output-service: sending response");
#endif

      respond({});
    };

    return {
        ids::output,
        index,
        handler};
  }
}

#endif
