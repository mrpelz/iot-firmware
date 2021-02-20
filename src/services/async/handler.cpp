#include "./handler.h"

namespace IotNode {

uint32_t requestTime = 0;
bool requestRunning = false;
UDP::RespondCallback responseCallback;

void asyncHandler(
    UDP::Payload *request,
    UDP::RespondCallback respond
) {
  Log::debug("async-service", "got request");

  requestTime = millis();
  requestRunning = true;
  responseCallback = [respond](UDP::Payload response) {
    respond(response);
  };
}

void asyncUpdate() {
  if (!requestRunning) return;

  uint32_t timeSinceRequest = millis() - requestTime;
  if (timeSinceRequest < ASYNC_RESPONSE_DELAY) return;
  requestTime = 0;
  requestRunning = false;

  Log::debug("async-service", "sending delayed response");

  responseCallback({ 0x0a, 0x0b, 0x0c });
}

} // project namespace
