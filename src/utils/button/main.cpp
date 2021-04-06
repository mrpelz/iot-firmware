#include "./main.h"

#ifdef IOT_NODE_BUTTONS

namespace IotNode {
namespace Utils {

namespace Button {
  Class::Class(Config _config) {
    config = _config;
  }

  void Class::setChangeCallback(ChangeCallback callback) {
    state.changeCallback = callback;
  }

  void Class::setDebug(Utils::Log::Callback callback) {
    state.debugCallback = callback;
  }

  void Class::start() {
    state.running = true;

    pinMode(
      config.pin,
      config.pullupEnable ? INPUT_PULLUP : INPUT
    );
  }

  void Class::stop() {
    state.running = false;

    pinMode(
      config.pin,
      INPUT
    );
  }

  void Class::update() {
    if (!state.running) return;

    auto now = millis();

    bool rawDown = digitalRead(config.pin);
    auto down = config.invert ? rawDown : !rawDown;

    if (config.noiseGateTime) {
      if (!state.noiseGateTime && down) {
        state.noiseGateTime = now;
        return;
      }

      auto timeSinceNoiseGateBegin = now - state.noiseGateTime;
      if (timeSinceNoiseGateBegin < config.noiseGateTime && down) {
        return;
      }

      if (state.noiseGateTime && !down) {
        state.noiseGateTime = 0;
        return;
      }

      state.noiseGateTime = 0;
    }

    bool downChanged = down != state.down;

    auto timeSinceLastChange = now - state.changeTime;
    if (timeSinceLastChange < config.debounceTime) return;

    auto longpressChanged = false;

    if (!down || (down && downChanged)) {
      state.longpress = 0;
      state.longpressTime = now;
    } else {
      auto timeSinceLastLongpress = now - state.longpressTime;

      if (timeSinceLastLongpress > config.longpressTime) {
        longpressChanged = true;
        state.longpress = state.longpress + 1;
        state.longpressTime = now;
      }
    }

    if (downChanged) {
      state.changeTime = now;
      state.down = down;

      if (down) {
        if (timeSinceLastChange < config.repeatTime) {
          state.repeat = state.repeat + 1;
        } else {
          state.repeat = 0;
        }
      }
    }

    if (!(downChanged || longpressChanged)) return;

    state.debugCallback("event", "button");
    state.debugCallback("button.down", String(state.down));
    state.debugCallback("button.down.changed", String(downChanged));
    state.debugCallback("button.down.change-period", String(timeSinceLastChange));
    state.debugCallback("button.repeat", String(state.repeat));
    state.debugCallback("button.longpress", String(state.longpress));

    state.changeCallback({
      state.down,
      downChanged,
      timeSinceLastChange,
      state.repeat,
      state.longpress
    });
  }
}

} // section namespace
} // project namespace

#endif
