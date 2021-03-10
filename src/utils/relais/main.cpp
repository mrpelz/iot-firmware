#include "./main.h"

namespace IotNode {
namespace Utils {

namespace Relais {
  Class::Class(Config config) {
    state.pin = config.pin;
    state.invert = config.invert;
  }

  void Class::commit() {
    if (!state.wasInitialized) return;

    Log::debug("relais", "commit");

    digitalWrite(state.pin, (state.invert ? !state.on : state.on));
  }

  bool Class::isOn() {
    return state.on;
  }

  void Class::init() {
    Log::debug("relais", "init");
    Log::debug("relais.pin", String(state.pin));

    pinMode(state.pin, OUTPUT);
    state.wasInitialized = true;

    commit();
  }

  void Class::setOn(bool on) {
    Log::debug("relais.set-on", on ? "on" : "off");
    Log::debug("relais.pin", String(state.pin));

    state.on = on;
    commit();
  }

  void Class::toggle() {
    Log::debug("relais.toggle", state.on ? "on2off" : "off2on");
    Log::debug("relais.pin", String(state.pin));

    state.on = !state.on;
    commit();
  }
}

} // section namespace
} // project namespace
