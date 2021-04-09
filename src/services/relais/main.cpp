#include "./main.h"

#ifdef IOT_NODE_RELAIS

namespace IotNode {
namespace Services {

namespace Relais {
  Utils::UDP::Service makeService(Utils::Relais::Regular *relais, uint8_t index) {
    uint8_t serviceId = ids::relais + index;

    auto handler = [relais, index](
      Utils::UDP::Payload *request,
      Utils::UDP::RespondCallback respond,
      Utils::UDP::Peer peer
    ) {
      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("relais-service", "got request");
        Utils::Log::debug("relais-service.index", String(index));
      #endif

      bool on = request->size() >= 1 && request->at(0) != 0;

      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("relais-service.on", String(on));
      #endif

      relais->setOn(on);

      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("relais-service", "sending response");
      #endif

      respond({});
    };

    return {
      serviceId,
      handler
    };
  }

  Utils::UDP::Service makeService(Utils::Relais::Pulse *relais, uint8_t index) {
    uint8_t serviceId = ids::relais + index;

    auto handler = [relais, index](
      Utils::UDP::Payload *request,
      Utils::UDP::RespondCallback respond,
      Utils::UDP::Peer peer
    ) {
      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("relais-service", "got request");
        Utils::Log::debug("relais-service.index", String(index));
      #endif

      bool on = request->size() >= 1 && request->at(0) != 0;

      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("relais-service.on", String(on));
      #endif

      relais->setOn(on);

      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("relais-service", "sending response");
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
