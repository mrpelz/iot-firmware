#include "./handler.h"

Relais::Relais(RelaisConfig config) {
  state.pin = config.pin;
  state.invert = config.invert;
}

void Relais::commit() {
  if (!state.wasInitialized) return;

  debug("relais", "commit");

  digitalWrite(state.pin, (state.invert ? !state.on : state.on));
}

bool Relais::isOn() {
  return state.on;
}

void Relais::init() {
  debug("relais", "init");
  debug("relais.pin", String(state.pin));

  pinMode(state.pin, OUTPUT);
  state.wasInitialized = true;

  commit();
}

void Relais::setOn(bool on) {
  debug("relais.set-on", on ? "true" : "false");
  debug("relais.pin", String(state.pin));

  state.on = on;
  commit();
}

void Relais::toggle() {
  debug("relais.toggle", state.on ? "on2off" : "off2on");
  debug("relais.pin", String(state.pin));

  state.on = !state.on;
  commit();
}
