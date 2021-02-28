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

  void Class::tick() {
    if (!state.running) {
      start();
    }

    state.ticked = true;
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

      restartOnTimeout->tick();

      Log::debug("keepalive-service", "sending response");

      respond({});
    };

    return handler;
  }
}

} // project namespace
