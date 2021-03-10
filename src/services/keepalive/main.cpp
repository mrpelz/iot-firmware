#include "./main.h"

namespace IotNode {
namespace Services {

namespace Keepalive {
  Class::Class(unsigned long timeout) {
    state.timeout = timeout;
  }

  void Class::start() {
    Utils::Log::debug("keepalive", "start");
    state.running = true;
  }

  void Class::stop() {
    Utils::Log::debug("keepalive", "stop");
    state.running = false;
  }

  void Class::tick(bool forceRestart) {
    if (!state.running) {
      start();
    }

    state.ticked = true;
    state.forceRestart = forceRestart;
  }

  void Class::update() {
    unsigned long now = millis();

    if (state.ticked) {
      state.ticked = false;

      Utils::Log::debug("keepalive", "tick");
      state.lastTick = now;
    }

    if (!state.running) return;

    unsigned long timeSinceTick = now - state.lastTick;

    if (state.forceRestart) {
      Utils::Log::debug("keepalive", "force restart");
      ESP.restart();
    }

    if (timeSinceTick > state.timeout) {
      Utils::Log::debug("keepalive", "trip");
      ESP.restart();
    }
  }

  Utils::UDP::RequestHandler makeHandler(Class *restartOnTimeout) {
    auto handler = [restartOnTimeout](
      Utils::UDP::Payload *request,
      Utils::UDP::RespondCallback respond
    ) {
      Utils::Log::debug("keepalive-service", "got request");

      auto restart = request->size() >= 1 && request->at(0) != 0;
      Utils::Log::debug("keepalive-service.restart", restart ? "true" : "false");

      restartOnTimeout->tick(restart);

      Utils::Log::debug("keepalive-service", "sending response");

      respond({});
    };

    return handler;
  }
}

} // section namespace
} // project namespace
