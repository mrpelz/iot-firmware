#include "./handler.h"

uint32_t requestTime = 0;
bool requestRunning = false;
std::function<void (std::vector<uint8_t> response)> responseCallback;

void asyncHandler(
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
) {
  debug("async-service", "got request");

  requestTime = millis();
  requestRunning = true;
  responseCallback = [respond](std::vector<uint8_t> response) {
    respond(response);
  };
}

void asyncUpdate() {
  if (!requestRunning) return;

  uint32_t timeSinceRequest = millis() - requestTime;
  if (timeSinceRequest < ASYNC_RESPONSE_DELAY) return;
  requestTime = 0;
  requestRunning = false;

  debug("async-service", "sending delayed response");

  responseCallback({ 0x0a, 0x0b, 0x0c });
}
