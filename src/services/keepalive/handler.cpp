#include "./handler.h"

RestartOnTimeout::RestartOnTimeout(unsigned long timeout) {
  state.timeout = timeout;
  debug("keepalive.timeout", String(state.timeout));
}

void RestartOnTimeout::start() {
  debug("event", "keepalive.start");
  state.running = true;
}

void RestartOnTimeout::stop() {
  debug("event", "keepalive.stop");
  state.running = false;
}

void RestartOnTimeout::tick() {
  if (!state.running) {
    start();
  }

  state.ticked = true;
}

void RestartOnTimeout::update() {
  unsigned long now = millis();

  if (state.ticked) {
    state.ticked = false;

    debug("event", "keepalive.tick");
    state.lastTick = now;
  }

  if (!state.running) return;

  unsigned long timeSinceTick = now - state.lastTick;

  if (timeSinceTick > state.timeout) {
    debug("event", "keepalive.trip");
    ESP.restart();
  }
}

RestartOnTimeout restartOnTimeout(120000);

void keepAliveHandler(
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
) {
  debug("event", "udp-service.keepalive");

  restartOnTimeout.tick();

  respond({});
}

void timeoutUpdate() {
  restartOnTimeout.update();
}
