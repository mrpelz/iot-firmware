#include "./main.h"

namespace IotNode {

namespace Indicator {
  Class::Class(Config config) {
    state.pin = config.pin;
    state.invert = config.invert;
    state.blinkPeriod = config.blinkPeriod;
  }

  void Class::commit() {
    if (!state.wasInitialized) return;

    digitalWrite(state.pin, (state.invert ? !state.on : state.on));
  }

  bool Class::isOn() {
    return state.on;
  }

  void Class::blink(void) {
    Log::debug("indicator.blink", "infinite");
    Log::debug("indicator.pin", String(state.pin));

    state.on = false;

    state.blinkChange = millis();
    state.blinkCount = 0;
    state.blinkInfinite = true;
  }

  void Class::blink(uint8_t count) {
    Log::debug("indicator.blink", String(count));
    Log::debug("indicator.pin", String(state.pin));

    state.on = false;

    state.blinkChange = millis();
    state.blinkCount = count * 2;
    state.blinkInfinite = false;
  }

  void Class::init() {
    Log::debug("indicator", "init");
    Log::debug("indicator.pin", String(state.pin));

    pinMode(state.pin, OUTPUT);
    state.wasInitialized = true;

    commit();
  }

  void Class::setOn(bool on) {
    Log::debug("indicator.set-on", on ? "on" : "off");
    Log::debug("indicator.pin", String(state.pin));

    state.on = on;

    // abort blink
    state.blinkCount = 0;
    state.blinkInfinite = false;

    commit();
  }

  void Class::toggle() {
    Log::debug("indicator.toggle", state.on ? "on2off" : "off2on");
    Log::debug("indicator.pin", String(state.pin));

    state.on = !state.on;

    // abort blink
    state.blinkCount = 0;
    state.blinkInfinite = false;

    commit();
  }

  void Class::update() {
    if (!state.blinkCount && !state.blinkInfinite) return;

    unsigned long now = millis();
    unsigned long timeSinceLastBlink = now - state.blinkChange;

    if (timeSinceLastBlink < state.blinkPeriod) return;

    state.blinkChange = now;
    state.on = !state.on;

    if (!state.blinkInfinite) {
      state.blinkCount = state.blinkCount - 1;
    }

    commit();
  }
}

} // project namespace
