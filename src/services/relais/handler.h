#ifndef _RELAIS_HANDLER
#define _RELAIS_HANDLER

#define TOSTRING(x) #x
#define STR(x) TOSTRING(x)

#include <Arduino.h>
#include "../../logging.h"

typedef std::function<void (
  std::vector<uint8_t> *request,
  std::function<void (std::vector<uint8_t> response)> respond
)> RelaisHandler;

RelaisHandler makeRelaisHandler(uint8_t pin, bool invert);

#endif
