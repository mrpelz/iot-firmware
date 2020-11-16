#ifndef _ASYNC_HANDLER
#define _ASYNC_HANDLER

#include <Arduino.h>
#include "../../udp-messaging.h"
#include "../../logging.h"

void asyncHandler(
    std::vector<uint8_t> *request,
    RespondCallback respond
);

void asyncUpdate();

#endif
