#include "./handler.h"

RelaisHandler makeRelaisHandler(uint8_t pin, bool invert) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, invert);

  RelaisHandler relaisHandler = [pin, invert](
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
  ) {
    debug("event", "udp-service.relais");
    debug("udp-service.relais.pin", String(pin));

    bool on = (*request)[0] != 0;

    digitalWrite(pin, (invert ? !on : on));

    uint8_t response = on ? 0x01 : 0x00;
    respond({ response });
  };

  return relaisHandler;
}
