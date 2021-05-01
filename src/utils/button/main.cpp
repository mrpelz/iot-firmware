#include "./main.h"

#ifdef IOT_NODE_BUTTONS

namespace IotNode {
namespace Utils {

namespace Button {
  std::vector<Class *> buttons;

  Class::Class(Config _config) {
    config = _config;
  }

  bool Class::isDown() {
    return state.down;
  }

  void Class::setChangeCallback(ChangeCallback callback) {
    state.changeCallback = callback;
  }

  void Class::start() {
    buttons.insert(buttons.end(), this);

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
    update(false);
  }
  void Class::update(bool force) {
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

    bool downChanged = force || (down != state.down);

    auto timeSinceLastChange = now - state.changeTime;
    if (!force && timeSinceLastChange < config.debounceTime) return;

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
      auto lastChangeTime = state.changeTime;

      state.changeTime = now;
      state.down = down;

      if (!force && down && lastChangeTime) {
        if (timeSinceLastChange < config.repeatTime) {
          state.repeat = state.repeat + 1;
        } else {
          state.repeat = 0;
        }
      }
    }

    if (!(downChanged || longpressChanged)) return;

    std::vector<bool> pressedMap;
    std::for_each(
      std::begin(buttons),
      std::end(buttons),
      [&pressedMap](Class *button) {
        pressedMap.insert(pressedMap.end(), button->isDown());
      }
    );

    #ifdef IOT_NODE_LOGGING
      Log::debug("event", "button");
      Log::debug("button.down", String(state.down));
      Log::debug("button.down.changed", String(downChanged));
      Log::debug("button.down.change-period", String(timeSinceLastChange));
      Log::debug("button.repeat", String(state.repeat));
      Log::debug("button.longpress", String(state.longpress));
    #endif

    state.changeCallback({
      state.down,
      downChanged,
      timeSinceLastChange,
      state.repeat,
      state.longpress,
      pressedMap
    });
  }
}

} // section namespace
} // project namespace

#endif
