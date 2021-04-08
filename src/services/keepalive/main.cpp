#include "./main.h"

namespace IotNode {
namespace Services {

namespace Keepalive {
  Utils::UDP::RequestHandler makeHandler() {
    auto handler = [](
      Utils::UDP::Payload *request,
      Utils::UDP::RespondCallback respond,
      Utils::UDP::Peer peer
    ) {
      Utils::Log::debug("keepalive-service", "got request");

      auto restart = request->size() >= 1 && request->at(0) != 0;
      if (restart) {
        Utils::Log::debug("keepalive-service", "force restart");
        ESP.restart();
      }

      Utils::Keepalive::keepalive.tick();
      Utils::Keepalive::eventPeer.tick();

      Utils::UDP::instance.setEventPeer(peer);

      Utils::Log::debug("keepalive-service", "sending response");

      respond({});
    };

    return handler;
  }
}

} // section namespace
} // project namespace
