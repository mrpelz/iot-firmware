#include "./handler.h"

RestartOnTimeout::RestartOnTimeout(uint32_t timeout) {
  state.timeout = timeout;
}

void RestartOnTimeout::start() {
  debug("keepalive", "start");
  state.running = true;
}

void RestartOnTimeout::stop() {
  debug("keepalive", "stop");
  state.running = false;
}

void RestartOnTimeout::tick() {
  if (!state.running) {
    start();
  }

  state.ticked = true;
}

void RestartOnTimeout::update() {
  uint32_t now = millis();

  if (state.ticked) {
    state.ticked = false;

    debug("keepalive", "tick");
    state.lastTick = now;
  }

  if (!state.running) return;

  uint32_t timeSinceTick = now - state.lastTick;

  if (timeSinceTick > state.timeout) {
    debug("keepalive", "trip");
    ESP.restart();
  }
}

RestartOnTimeout restartOnTimeout(120000);

void keepAliveHandler(
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
) {
  debug("keepalive-service", "got request");

  restartOnTimeout.tick();

  debug("keepalive-service", "sending response");

  respond({});
}

void timeoutUpdate() {
  restartOnTimeout.update();
}
