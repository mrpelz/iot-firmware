#include "./main.h"

#ifdef IOT_NODE_LED

namespace IotNode {
namespace Services {

namespace Led {
  Utils::UDP::Service makeService(Utils::Led::Class *led, uint8_t index) {
    uint8_t serviceId = ids::led + index;

    auto handler = [led, index](
      Utils::UDP::Payload *request,
      Utils::UDP::RespondCallback respond
    ) {
      Utils::Log::debug("led-service", "got request");
      Utils::Log::debug("led-service.index", String(index));

      auto duty = request->size() >= 2 ? ((uint16_t *)request->data())[0] : (uint16_t)0;

      Utils::Log::debug("led-service.duty", String(duty));

      led->setDutyCycle(duty);

      Utils::Log::debug("led-service", "sending response");

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
