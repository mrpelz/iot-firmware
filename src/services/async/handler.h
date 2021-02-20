#ifndef _ASYNC_HANDLER
#define _ASYNC_HANDLER

#define ASYNC_RESPONSE_DELAY 30000

#include <Arduino.h>
#include "../../utils/udp/main.h"
#include "../../utils/log.h"

namespace IotNode {

void asyncHandler(
    UDP::Payload *request,
    UDP::RespondCallback respond
);

void asyncUpdate();

} // project namespace

#endif
