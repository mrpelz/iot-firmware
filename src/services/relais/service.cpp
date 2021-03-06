#include "./service.h"

namespace IotNode {

namespace Relais {
  UDP::Service makeService(Class *relais, uint8_t index) {
    uint8_t serviceId = serviceIds::relais + index;

    auto handler = [relais, index](
      UDP::Payload *request,
      UDP::RespondCallback respond
    ) {
      bool on = request->size() >= 1 && request->at(0) != 0;

      Log::debug("event", "relais-service.switch");
      Log::debug("relais-service.index", String(index));
      Log::debug("relais-service.on", String(on));

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

} // project namespace
