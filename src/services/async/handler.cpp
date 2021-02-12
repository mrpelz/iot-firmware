#include "./handler.h"

unsigned long requestTime = 0;
std::function<void (std::vector<uint8_t> response)> responseCallback;

void asyncHandler(
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
) {
  debug("event", "udp-service.async.request");

  requestTime = millis();
  responseCallback = [respond](std::vector<uint8_t> response) {
    debug("event", "udp-service.async.response");
    respond(response);
  };
}

void asyncUpdate() {
  if (!requestTime) return;

  unsigned long timeSinceRequest = millis() - requestTime;
  if (timeSinceRequest < 30000) return;
  requestTime = 0;

  debug("event", "udp-service.async.delayed-response");
  responseCallback({ 0x0a, 0x0b, 0x0c });
}
