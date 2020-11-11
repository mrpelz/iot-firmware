#include "./service.h"

UDPService makeRelaisService(uint8_t index, uint8_t pin, bool invert) {
  uint8_t serviceId = serviceIds::relais0 + index;

  UDPService relaisService = {
    serviceId,
    makeRelaisHandler(pin, invert)
  };

  return relaisService;
}
