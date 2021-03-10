#include "./main.h"

namespace IotNode {
namespace Services {

namespace Relais {
  Class::Class(Config config) {
    state.pin = config.pin;
    state.invert = config.invert;
  }

  void Class::commit() {
    if (!state.wasInitialized) return;

    Utils::Log::debug("relais", "commit");

    digitalWrite(state.pin, (state.invert ? !state.on : state.on));
  }

  bool Class::isOn() {
    return state.on;
  }

  void Class::init() {
    Utils::Log::debug("relais", "init");
    Utils::Log::debug("relais.pin", String(state.pin));

    pinMode(state.pin, OUTPUT);
    state.wasInitialized = true;

    commit();
  }

  void Class::setOn(bool on) {
    Utils::Log::debug("relais.set-on", on ? "on" : "off");
    Utils::Log::debug("relais.pin", String(state.pin));

    state.on = on;
    commit();
  }

  void Class::toggle() {
    Utils::Log::debug("relais.toggle", state.on ? "on2off" : "off2on");
    Utils::Log::debug("relais.pin", String(state.pin));

    state.on = !state.on;
    commit();
  }
}

} // section namespace
} // project namespace
