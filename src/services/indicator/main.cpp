#include "./main.h"

#ifdef IOT_NODE_INDICATORS

namespace IotNode {
namespace Services {

namespace Indicator {
  Utils::UDP::Service makeService(Utils::Indicator::Class *indicator, uint8_t index) {
    uint8_t serviceId = ids::indicator + index;

    auto handler = [indicator, index](
      Utils::UDP::Payload *request,
      Utils::UDP::RespondCallback respond,
      Utils::UDP::Peer peer
    ) {
      Utils::Log::debug("indicator-service", "got request");
      
      if (request->size() < 1) {
        Utils::Log::debug("indicator-service", "missing parameters");
      }

      auto cmd = request->at(0);

      switch (cmd) {
      case INDICATOR_CMD_ON:
        indicator->setOn(true);
        break;
      case INDICATOR_CMD_OFF:
        indicator->setOn(false);
        break;
      case INDICATOR_CMD_BLINK:
        if (request->size() >= 2) {
          auto count = request->at(1);

          indicator->blink(count);
        } else {
          indicator->blink();
        }
      }

      Utils::Log::debug("indicator-service", "sending response");

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
