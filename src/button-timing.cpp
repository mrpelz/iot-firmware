#include "./button-timing.h"

ButtonTiming::ButtonTiming(ButtonTimingConfig config) {
  std::for_each(
    std::begin(config.buttons),
    std::end(config.buttons),
    [&](ButtonAttributes buttonConfig) {
      state.buttons.push_back({
        buttonConfig.index,
        Button(
          buttonConfig.pin,
          config.debounceTime,
          buttonConfig.pullupEnable,
          buttonConfig.invert
        ),
        false,
        0
      });
    }
  );
}

void ButtonTiming::begin() {
  state.begun = true;

  std::for_each(
    std::begin(state.buttons),
    std::end(state.buttons),
    [&](ButtonState button) {
      button.instance.begin();
    }
  );
}

void ButtonTiming::setChangeCallback(
  std::function<void (
    uint8_t index,
    bool isPressed,
    uint32_t prevDuration
  )> callback
) {
  state.changeCallback = callback;
}

void ButtonTiming::setDebug(std::function<void (const String &, const String &)> callback) {
  state.debugCallback = callback;
}

void ButtonTiming::update() {
  if (!state.begun) return;

  std::for_each(
    std::begin(state.buttons),
    std::end(state.buttons),
    [&](ButtonState button) {
      button.instance.read();

      uint32_t lastChange = button.instance.lastChange();

      if (lastChange == button.lastChange) return;
      button.lastChange = lastChange;

      if (button.instance.wasPressed()) {
        button.isPressed = true;
        uint32_t releasedFor = lastChange - button.lastChange;

        state.debugCallback("event", "button.down");
        state.debugCallback("button.down.index", String(button.index));
        state.debugCallback("button.down.time", String(lastChange));
        state.debugCallback("button.down.prev-duration", String(releasedFor));

        state.changeCallback(button.index, button.isPressed, releasedFor);

        return;
      }

      if (button.instance.wasReleased()) {
        button.isPressed = false;
        uint32_t pressedFor = lastChange - button.lastChange;

        state.debugCallback("event", "button.up");
        state.debugCallback("button.up.index", String(button.index));
        state.debugCallback("button.up.time", String(lastChange));
        state.debugCallback("button.up.prev-duration", String(pressedFor));

        state.changeCallback(button.index, button.isPressed, pressedFor);
      }
    }
  );
}
