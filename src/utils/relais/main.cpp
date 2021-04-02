#include "./main.h"

#ifdef IOT_NODE_RELAIS

namespace IotNode {
namespace Utils {

namespace Relais {
  Regular::Regular(RegularConfig _config) {
    config = _config;
  }

  void Regular::commit() {
    if (!state.wasInitialized) return;

    Log::debug("relais", "commit");

    digitalWrite(config.pin, (config.invert ? !state.on : state.on));
  }

  bool Regular::isOn() {
    return state.on;
  }

  void Regular::init() {
    Log::debug("relais", "init");
    Log::debug("relais.pin", String(config.pin));

    pinMode(config.pin, OUTPUT);
    state.wasInitialized = true;

    commit();
  }

  void Regular::setOn(bool on) {
    Log::debug("relais.set-on", on ? "on" : "off");
    Log::debug("relais.pin", String(config.pin));

    state.on = on;
    commit();
  }

  void Regular::toggle() {
    Log::debug("relais.toggle", state.on ? "on2off" : "off2on");
    Log::debug("relais.pin", String(config.pin));

    state.on = !state.on;
    commit();
  }

  Pulse::Pulse(PulseConfig _config) {
    config = _config;
  }

  void Pulse::commit() {
    if (!state.wasInitialized) return;

    Log::debug("relais", "commit");

    auto unactionablePin = state.on ? config.offPin : config.onPin;
    auto actionablePin = state.on ? config.onPin : config.offPin;

    digitalWrite(unactionablePin, config.invert);
    digitalWrite(actionablePin, !config.invert);
  }

  bool Pulse::isOn() {
    return state.on;
  }

  void Pulse::init() {
    Log::debug("relais", "init");
    Log::debug("relais.on-pin", String(config.onPin));
    Log::debug("relais.off-pin", String(config.offPin));

    pinMode(config.onPin, OUTPUT);
    pinMode(config.offPin, OUTPUT);
    state.wasInitialized = true;

    commit();
  }

  void Pulse::setOn(bool on) {
    Log::debug("relais.set-on", on ? "on" : "off");
    Log::debug("relais.on-pin", String(config.onPin));
    Log::debug("relais.off-pin", String(config.offPin));

    state.on = on;
    commit();
  }

  void Pulse::toggle() {
    Log::debug("relais.toggle", state.on ? "on2off" : "off2on");
    Log::debug("relais.on-pin", String(config.onPin));
    Log::debug("relais.off-pin", String(config.offPin));

    state.on = !state.on;
    commit();
  }
}

} // section namespace
} // project namespace

#endif
