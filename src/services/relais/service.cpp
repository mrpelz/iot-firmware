#include "./service.h"

namespace IotNode {
namespace Services {

namespace Relais {
  Utils::UDP::Service makeService(Class *relais, uint8_t index) {
    uint8_t serviceId = serviceIds::relais + index;

    auto handler = [relais, index](
      Utils::UDP::Payload *request,
      Utils::UDP::RespondCallback respond
    ) {
      bool on = request->size() >= 1 && request->at(0) != 0;

      Utils::Log::debug("event", "relais-service.switch");
      Utils::Log::debug("relais-service.index", String(index));
      Utils::Log::debug("relais-service.on", String(on));

      relais->setOn(on);

      uint8_t response = on ? 0x01 : 0x00;
      respond({ response });
    };

    return {
      serviceId,
      handler
    };
  }
}

} // section namespace
} // project namespace
