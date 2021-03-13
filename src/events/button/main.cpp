#include "./main.h"

namespace IotNode {
namespace Events {

namespace Button {
  Utils::Button::ChangeCallback makeEvent(Utils::UDP::Class *udp, uint8_t index) {
    uint8_t serviceId = ids::button + index;

    auto handler = [udp, serviceId](Utils::Button::Update update) {
      Utils::Log::debug("button-event", "triggered");

      std::vector<uint8_t> response = {
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

      udp->event(serviceId, response);
    };

    return handler;
  }
}

} // section namespace
} // project namespace
