#include "./event.h"

void buttonEvent(UDPMessaging *udp, ButtonUpdate update) {
  debug("event", "button-event");

  std::vector<uint8_t> response = {
    update.index,
    (uint8_t)(update.down ? 0x01 : 0x00),
    (uint8_t)(update.downChanged ? 0x01 : 0x00),
    update.repeat,
    update.longpress,
  };

  response.insert(
    response.end(),
    &update.prevDuration,
    &update.prevDuration + sizeof(update.prevDuration)
  );

  udp->event(eventIds::button, response);
}
