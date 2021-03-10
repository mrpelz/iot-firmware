#include "./main.h"

namespace IotNode {
namespace Events {

namespace Button {
  Class::Class(Config config) {
    state.debounceTime = config.debounceTime;
    state.repeatTime = config.repeatTime;
    state.longpressTime = config.longpressTime;

    std::for_each(
      std::begin(config.buttons),
      std::end(config.buttons),
      [&](SingleAttributes attributes) {
        state.buttons.push_back({ attributes, false, 0, 0, 0, 0 });
      }
    );
  }

  void Class::eachButton(EachCallback callback) {
    for (
      std::vector<SingleState>::iterator iterator = std::begin(state.buttons);
      iterator != std::end(state.buttons);
      ++iterator
    ) {
      callback(iterator.base());
    }
  }

  void Class::setChangeCallback(ChangeCallback callback) {
    state.changeCallback = callback;
  }

  void Class::setDebug(Utils::Log::Callback callback) {
    state.debugCallback = callback;
  }

  void Class::start() {
    state.running = true;

    eachButton([](SingleState *button) {
      pinMode(
        button->attributes.pin,
        button->attributes.pullupEnable ? INPUT_PULLUP : INPUT
      );
    });
  }

  void Class::stop() {
    state.running = false;

    eachButton([](SingleState *button) {
      pinMode(
        button->attributes.pin,
        INPUT
      );
    });
  }

  void Class::update() {
    if (!state.running) return;

    auto now = millis();

    eachButton([&](SingleState *button) {
      bool rawDown = digitalRead(button->attributes.pin);

      bool down = button->attributes.invert ? !rawDown : rawDown;
      bool downChanged = down != button->down;

      auto timeSinceLastChange = now - button->changeTime;
      if (timeSinceLastChange < state.debounceTime) return;

      bool longpressChanged = false;

      if (!down || (down && downChanged)) {
        button->longpress = 0;
        button->longpressTime = now;
      } else {
        auto timeSinceLastLongpress = now - button->longpressTime;

        if (timeSinceLastLongpress > state.longpressTime) {
          longpressChanged = true;
          button->longpress = button->longpress + 1;
          button->longpressTime = now;
        }
      }

      if (downChanged) {
        button->changeTime = now;
        button->down = down;

        if (down) {
          if (timeSinceLastChange < state.repeatTime) {
            button->repeat = button->repeat + 1;
          } else {
            button->repeat = 0;
          }
        }
      }

      if (!(downChanged || longpressChanged)) return;

      state.debugCallback("event", "button");
      state.debugCallback("button.index", String(button->attributes.index));
      state.debugCallback("button.down", String(button->down));
      state.debugCallback("button.down.changed", String(downChanged));
      state.debugCallback("button.down.change-period", String(timeSinceLastChange));
      state.debugCallback("button.repeat", String(button->repeat));
      state.debugCallback("button.longpress", String(button->longpress));

      state.changeCallback({
        button->attributes.index,
        button->down,
        downChanged,
        timeSinceLastChange,
        button->repeat,
        button->longpress
      });
    });
  }
}

} // section namespace
} // project namespace
