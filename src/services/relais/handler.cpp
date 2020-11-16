#include "./handler.h"

Relais::Relais(RelaisConfig config) {
  state.pin = config.pin;
  state.invert = config.invert;
}

void Relais::commit() {
  if (!state.wasInitialized) return;

  debug("event", "relais.switch");
  debug("relais.pin", String(state.pin));
  debug("relais.on", String(state.on));

  digitalWrite(state.pin, (state.invert ? !state.on : state.on));
}

bool Relais::isOn() {
  return state.on;
}

void Relais::init() {
  debug("event", "relais.init");
  debug("relais.pin", String(state.pin));

  pinMode(state.pin, OUTPUT);
  state.wasInitialized = true;

  commit();
}

void Relais::setOn(bool on) {
  state.on = on;
  commit();
}

void Relais::toggle() {
  state.on = !state.on;
  commit();
}
