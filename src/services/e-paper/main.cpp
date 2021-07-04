#include "./main.h"

#ifdef IOT_NODE_E_PAPER

namespace IotNode {
namespace Services {

namespace EPaper {
  Utils::UDP::RequestHandler makeHandler() {
    auto handler = [](
      Utils::UDP::Payload *request,
      Utils::UDP::RespondCallback respond,
      Utils::UDP::Peer peer
    ) {
      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("e-paper-service", "got request");
      #endif

      auto on = request->size() >= 1 && request->at(0) != 0;
      if (on) {
        #ifdef IOT_NODE_LOGGING
          Utils::Log::debug("e-paper-service", "on");
        #endif

        // do something
      } else {
        #ifdef IOT_NODE_LOGGING
          Utils::Log::debug("e-paper-service", "off");
        #endif

        // do something
      }

      #ifdef IOT_NODE_LOGGING
        Utils::Log::debug("e-paper-service", "sending response");
      #endif

      respond({});
    };

    return handler;
  }
}

} // section namespace
} // project namespace

#endif
