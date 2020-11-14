#include "./service.h"

RelaisServiceResult makeRelaisService(uint8_t index, uint8_t pin, bool invert) {
  uint8_t serviceId = serviceIds::relais0 + index;
  auto relais = makeRelais(pin, invert);

  RelaisServiceResult result = {
    {
      serviceId,
      relais.handler
    },
    relais.init,
    relais.override
  };

  return result;
}
