#include "./main.h"

#ifdef IOT_NODE_RELAIS

namespace IotNode {
namespace Utils {

namespace Relais {
  Class::Class(Config _config) {
    config = _config;
  }

  void Class::commit() {
    if (!state.wasInitialized) return;

    Log::debug("relais", "commit");

    digitalWrite(config.pin, (config.invert ? !state.on : state.on));
  }

  bool Class::isOn() {
    return state.on;
  }

  void Class::init() {
    Log::debug("relais", "init");
    Log::debug("relais.pin", String(config.pin));

    pinMode(config.pin, OUTPUT);
    state.wasInitialized = true;

    commit();
  }

  void Class::setOn(bool on) {
    Log::debug("relais.set-on", on ? "on" : "off");
    Log::debug("relais.pin", String(config.pin));

    state.on = on;
    commit();
  }

  void Class::toggle() {
    Log::debug("relais.toggle", state.on ? "on2off" : "off2on");
    Log::debug("relais.pin", String(config.pin));

    state.on = !state.on;
    commit();
  }
}

} // section namespace
} // project namespace

#endif
