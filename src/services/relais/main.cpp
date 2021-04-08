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
      Utils::Log::debug("relais-service", "got request");
      Utils::Log::debug("relais-service.index", String(index));

      bool on = request->size() >= 1 && request->at(0) != 0;

      Utils::Log::debug("relais-service.on", String(on));

      relais->setOn(on);

      Utils::Log::debug("relais-service", "sending response");
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
      Utils::Log::debug("relais-service", "got request");
      Utils::Log::debug("relais-service.index", String(index));

      bool on = request->size() >= 1 && request->at(0) != 0;

      Utils::Log::debug("relais-service.on", String(on));

      relais->setOn(on);

      Utils::Log::debug("relais-service", "sending response");
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
