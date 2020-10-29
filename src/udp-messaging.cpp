#include <Arduino.h>

struct IncomingMessage {
  uint8_t requestId;
  uint8_t serviceId;
  uint8_t payload;
};

class UDPMessaging {
  public:
    UDPMessaging() {}
};
