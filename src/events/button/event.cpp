#include "./event.h"

void buttonEvent(
  UDPMessaging *udp,
  uint8_t index,
  bool down,
  bool downChanged,
  uint8_t repeat,
  uint8_t longpress,
  unsigned long prevDuration
) {
  debug("event", "button-event");

  std::vector<uint8_t> response = {
    index,
    (uint8_t)(down ? 0x01 : 0x00),
    (uint8_t)(downChanged ? 0x01 : 0x00),
    repeat,
    longpress
  };

  response.insert(
    response.end(),
    &prevDuration,
    &prevDuration + sizeof(prevDuration)
  );

  udp->event(eventIds::button, response);
}
