#include "./main.h"

#ifdef IOT_NODE_INDICATORS

namespace IotNode {
namespace Utils {

namespace Indicator {
  Class::Class(Config _config) {
    config = _config;
  }

  void Class::commit() {
    if (!state.wasInitialized) return;

    digitalWrite(config.pin, (config.invert ? !state.on : state.on));
  }

  bool Class::isOn() {
    return state.on;
  }

  void Class::blink() {
    #ifdef IOT_NODE_LOGGING
      Log::debug("indicator.blink", "infinite");
      Log::debug("indicator.pin", String(config.pin));
    #endif

    state.on = false;

    state.blinkChange = millis();
    state.blinkCount = 0;
    state.blinkInfinite = true;
  }

  void Class::blink(uint8_t count) {
    #ifdef IOT_NODE_LOGGING
      Log::debug("indicator.blink", String(count));
      Log::debug("indicator.pin", String(config.pin));
    #endif

    state.on = false;

    state.blinkChange = millis();
    state.blinkCount = count * 2;
    state.blinkInfinite = false;
  }

  void Class::init() {
    #ifdef IOT_NODE_LOGGING
      Log::debug("indicator", "init");
      Log::debug("indicator.pin", String(config.pin));
    #endif

    pinMode(config.pin, OUTPUT);
    state.wasInitialized = true;

    commit();
  }

  void Class::setBlinkFrequency(unsigned long blinkPeriodOn, unsigned long blinkPeriodOff) {
    config.blinkPeriodOn = blinkPeriodOn;
    config.blinkPeriodOff = blinkPeriodOff;
  }

  void Class::setOn(bool on) {
    #ifdef IOT_NODE_LOGGING
      Log::debug("indicator.set-on", on ? "on" : "off");
      Log::debug("indicator.pin", String(config.pin));
    #endif

    state.on = on;

    // abort blink
    state.blinkCount = 0;
    state.blinkInfinite = false;

    commit();
  }

  void Class::toggle() {
    #ifdef IOT_NODE_LOGGING
      Log::debug("indicator.toggle", state.on ? "on2off" : "off2on");
      Log::debug("indicator.pin", String(config.pin));
    #endif

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
      ? config.blinkPeriodOn
      : config.blinkPeriodOff;

    if (timeSinceLastBlink < relevantBlinkPeriod) return;

    state.blinkChange = now;
    state.on = !state.on;

    if (!state.blinkInfinite) {
      state.blinkCount = state.blinkCount - 1;
    }

    commit();
  }
}

} // section namespace
} // project namespace

#endif
