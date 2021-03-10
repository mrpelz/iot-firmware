#include "./main.h"

namespace IotNode {

namespace Indicator {
  Class::Class(Config config) {
    state.pin = config.pin;
    state.invert = config.invert;
    state.blinkPeriodOn = config.blinkPeriodOn;
    state.blinkPeriodOff = config.blinkPeriodOff;
  }

  void Class::commit() {
    if (!state.wasInitialized) return;

    digitalWrite(state.pin, (state.invert ? !state.on : state.on));
  }

  bool Class::isOn() {
    return state.on;
  }

  void Class::blink() {
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

  void Class::setBlinkFrequency(unsigned long blinkPeriodOn, unsigned long blinkPeriodOff) {
    state.blinkPeriodOn = blinkPeriodOn;
    state.blinkPeriodOff = blinkPeriodOff;
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
    unsigned long relevantBlinkPeriod = state.on
      ? state.blinkPeriodOn
      : state.blinkPeriodOff;

    if (timeSinceLastBlink < relevantBlinkPeriod) return;

    state.blinkChange = now;
    state.on = !state.on;

    if (!state.blinkInfinite) {
      state.blinkCount = state.blinkCount - 1;
    }

    commit();
  }
}

} // project namespace
