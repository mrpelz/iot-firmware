#include "./handler.h"

RelaisResult makeRelais(uint8_t pin, bool invert) {
  return {
    [pin, invert](
      std::vector<uint8_t> *request,
      std::function<void (std::vector<uint8_t> response)> respond
    ) {
      bool on = (*request)[0] != 0;

      debug("event", "relais.switch");
      debug("relais.pin", String(pin));
      debug("relais.on", String(on));

      digitalWrite(pin, (invert ? !on : on));

      uint8_t response = on ? 0x01 : 0x00;
      respond({ response });
    },
    [pin, invert]() {
      debug("event", "relais.init");
      debug("relais.pin", String(pin));

      pinMode(pin, OUTPUT);
      digitalWrite(pin, invert);
    },
    [pin, invert](bool on) {
      debug("event", "relais.override");
      debug("relais.pin", String(pin));
      debug("relais.on", String(on));

      digitalWrite(pin, (invert ? !on : on));
    }
  };
}
