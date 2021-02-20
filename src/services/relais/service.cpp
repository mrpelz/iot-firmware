#include "./service.h"

namespace IotNode {

UDPService makeRelaisService(Relais *relais, uint8_t index) {
  uint8_t serviceId = serviceIds::relais + index;

  auto handler = [relais, index](
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
  ) {
    bool on = (*request)[0] != 0;

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

} // project namespace
