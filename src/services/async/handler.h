#ifndef _ASYNC_HANDLER
#define _ASYNC_HANDLER

#define ASYNC_RESPONSE_DELAY 30000

#include <Arduino.h>
#include "../../utils/udp-messaging.h"
#include "../../utils/log.h"

void asyncHandler(
    std::vector<uint8_t> *request,
    RespondCallback respond
);

void asyncUpdate();

#endif
