#include "./main.h"

namespace IotNode {

namespace Keepalive {
  Class::Class(unsigned long timeout) {
    state.timeout = timeout;
  }

  void Class::start() {
    Log::debug("keepalive", "start");
    state.running = true;
  }

  void Class::stop() {
    Log::debug("keepalive", "stop");
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

      Log::debug("keepalive", "tick");
      state.lastTick = now;
    }

    if (!state.running) return;

    unsigned long timeSinceTick = now - state.lastTick;

    if (state.forceRestart) {
      Log::debug("keepalive", "force restart");
      ESP.restart();
    }

    if (timeSinceTick > state.timeout) {
      Log::debug("keepalive", "trip");
      ESP.restart();
    }
  }

  UDP::RequestHandler makeHandler(Class *restartOnTimeout) {
    auto handler = [restartOnTimeout](
      UDP::Payload *request,
      UDP::RespondCallback respond
    ) {
      Log::debug("keepalive-service", "got request");

      auto restart = request->size() >= 1 && request->at(0) != 0;
      Log::debug("keepalive-service.restart", restart ? "true" : "false");

      restartOnTimeout->tick(restart);

      Log::debug("keepalive-service", "sending response");

      respond({});
    };

    return handler;
  }
}

} // project namespace
