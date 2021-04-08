#include "./main.h"

namespace IotNode {
namespace Utils {

namespace Keepalive {
  Class::Class(unsigned long timeout, Callback callback) {
    state.timeout = timeout;
    state.callback = callback;
  }

  void Class::start() {
    Utils::Log::debug("keepalive", "start");
    state.running = true;
  }

  void Class::stop() {
    Utils::Log::debug("keepalive", "stop");
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

      Utils::Log::debug("keepalive", "tick");
      state.lastTick = now;
    }

    if (!state.running) return;

    unsigned long timeSinceTick = now - state.lastTick;

    if (timeSinceTick > state.timeout) {
      Utils::Log::debug("keepalive", "trip");
      state.callback();

      stop();
    }
  }
}

} // section namespace
} // project namespace
