#ifndef _ASYNC_HANDLER
#define _ASYNC_HANDLER

#include <Arduino.h>
#include "../../logging.h"

void asyncHandler(
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
);

void asyncUpdate();

#endif
