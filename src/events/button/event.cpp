#include "./event.h"

namespace IotNode {
namespace Events {

namespace Button {
  void buttonEvent(Utils::UDP::Class *udp, Update update) {
    Utils::Log::debug("button-event", "triggered");

    std::vector<uint8_t> response = {
      update.index,
      (uint8_t)(update.down ? 0x01 : 0x00),
      (uint8_t)(update.downChanged ? 0x01 : 0x00),
      update.repeat,
      update.longpress,
    };

    auto prevDuration = reinterpret_cast<uint8_t*>(&(update.prevDuration));

    response.insert(
      response.end(),
      prevDuration,
      prevDuration + sizeof(update.prevDuration)
    );

    Utils::Log::debug("button-event", "sending event");

    udp->event(ids::button, response);
  }
}

} // section namespace
} // project namespace
