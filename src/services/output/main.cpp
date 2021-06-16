#include "./main.h"

#ifdef IOT_NODE_OUTPUT

namespace IotNode {
namespace Services {

namespace Output {
  Utils::UDP::Service makeService(Utils::Output::Regular *output, uint8_t index) {
    uint8_t serviceId = ids::output + index;

    auto handler = [output, index](
      Utils::UDP::Payload *request,
      Utils::UDP::RespondCallback respond,
      Utils::UDP::Peer peer
    ) {
      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("output-service", "got request");
        Utils::Log::debug("output-service.index", String(index));
      #endif

      bool on = request->size() >= 1 && request->at(0) != 0;

      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("output-service.on", String(on));
      #endif

      output->setOn(on);

      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("output-service", "sending response");
      #endif

      respond({});
    };

    return {
      serviceId,
      handler
    };
  }

  Utils::UDP::Service makeService(Utils::Output::Pulse *output, uint8_t index) {
    uint8_t serviceId = ids::output + index;

    auto handler = [output, index](
      Utils::UDP::Payload *request,
      Utils::UDP::RespondCallback respond,
      Utils::UDP::Peer peer
    ) {
      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("output-service", "got request");
        Utils::Log::debug("output-service.index", String(index));
      #endif

      bool on = request->size() >= 1 && request->at(0) != 0;

      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("output-service.on", String(on));
      #endif

      output->setOn(on);

      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("output-service", "sending response");
      #endif

      respond({});
    };

    return {
      serviceId,
      handler
    };
  }
}

} // section namespace
} // project namespace

#endif
