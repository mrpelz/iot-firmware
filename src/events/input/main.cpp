#include "./main.h"

#ifdef IOT_NODE_INPUT

namespace IotNode {
namespace Events {

namespace Input {
  Class::Class(uint8_t _pin) {
    Class(_pin, false);
  }
  Class::Class(uint8_t _pin, bool _pullup) {
    Class(_pin, _pullup, 0);
  }
  Class::Class(uint8_t _pin, unsigned long _debounceTime) {
    Class(_pin, false, _debounceTime);
  }
  Class::Class(uint8_t _pin, bool _pullup, unsigned long _debounceTime) {
    pin = _pin;
    pullup = _pullup;
    debounceTime = _debounceTime;
  }

  void Class::setChangeCallback(ChangeCallback callback) {
    state.changeCallback = callback;
  }

  void Class::start() {
    state.running = true;

    pinMode(
      pin,
      pullup ? INPUT_PULLUP : INPUT
    );
  }

  void Class::update() {
    update(false);
  }
  void Class::update(bool force) {
    if (!state.running) return;

    auto now = millis();
    auto timeSinceDownChange = now - state.changeTime;
    if (!force && timeSinceDownChange < debounceTime) return;
    state.changeTime = now;

    bool down = digitalRead(pin);
    bool downChanged = force || down != state.down;

    if (downChanged) {
      state.down = down;
    }

    if (!downChanged) return;

    #ifdef IOT_NODE_LOGGING
      Utils::Log::debug("event", "input");
      Utils::Log::debug("input.down", String(state.down));
    #endif

    state.changeCallback(state.down);
  }

  ChangeCallback makeEvent(Utils::UDP::Class *udp, uint8_t index) {
    uint8_t serviceId = ids::input + index;

    auto handler = [udp, serviceId](bool down) {
      udp->event(serviceId, {
        (uint8_t)(down ? 0x01 : 0x00)
      });
    };

    return handler;
  }

  #ifdef IOT_NODE_ESP_NOW_NODE
    ChangeCallback makeEspNowEvent(uint8_t index) {
      uint8_t serviceId = ids::input + index;

      auto handler = [serviceId](bool down) {
        std::vector<uint8_t> response = {
          serviceId,
          (uint8_t)(down ? 0x01 : 0x00)
        };

        Utils::EspNowNode::send(response);
      };

      return handler;
    }
  #endif
}

} // section namespace
} // project namespace

#endif
