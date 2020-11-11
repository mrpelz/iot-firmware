#include "./event.h"

void buttonEvent(
  UDPMessaging *udp,
  uint8_t index,
  bool isPressed,
  uint32_t prevDuration
) {
  debug("event", "button-event");

  std::vector<uint8_t> response = {
    index,
    (uint8_t)(isPressed ? 0x01 : 0x00)
  };

  response.insert(
    response.end(),
    &prevDuration,
    &prevDuration + sizeof(prevDuration)
  );

  udp->event(eventIds::button, response);
}
