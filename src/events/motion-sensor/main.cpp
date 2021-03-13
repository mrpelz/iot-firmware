#include "./main.h"

namespace IotNode {
namespace Events {

namespace MotionSensor {
  Class::Class(uint8_t _pin) {
    pin = _pin;
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
      pin,
      INPUT
    );
  }

  void Class::update() {
    if (!state.running) return;

    bool down = digitalRead(pin);

    bool downChanged = down != state.down;

    if (downChanged) {
      state.down = down;
    }

    if (!downChanged) return;

    state.debugCallback("event", "motion");
    state.debugCallback("motion.down", String(state.down));

    state.changeCallback(state.down);
  }

  ChangeCallback makeEvent(Utils::UDP::Class *udp, uint8_t index) {
    uint8_t serviceId = ids::motionSensor + index;

    auto handler = [udp, serviceId](bool down) {
      udp->event(serviceId, {
        (uint8_t)(down ? 0x01 : 0x00)
      });
    };

    return handler;
  }
}

} // section namespace
} // project namespace
