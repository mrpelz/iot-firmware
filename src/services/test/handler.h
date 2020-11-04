#ifndef _TEST_HANDLER
#define _TEST_HANDLER

#include <Arduino.h>
#include "./logging.h"

void handler(
    std::vector<uint8_t> *request,
    std::function<void (std::vector<uint8_t> response)> respond
);

#endif
