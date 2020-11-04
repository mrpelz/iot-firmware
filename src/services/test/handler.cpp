#include "./handler.h"

void handler(
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
) {
  debug("event", "udp-service");

  std::for_each(
    std::begin(*request),
    std::end(*request),
    [&](uint8_t byte) {
      debug("udp-service.request.byte", String(byte, HEX));
    }
  );

  respond({
    0x00,
    0xDE,
    0xAD,
    0xBE,
    0xEF,
    0xFF
  });
}
