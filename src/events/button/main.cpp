#include "./main.h"

#ifdef IOT_NODE_BUTTONS

namespace IotNode::Events::Button
{
  Utils::Button::ChangeCallback makeEvent(Utils::UDP::Class *udp, unsigned char index)
  {
    auto handler = [udp, index](Utils::Button::Update update)
    {
#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("button-event: triggered");
#endif

      ::std::vector<unsigned char> payload = {
          (unsigned char)(update.down ? 0x01 : 0x00),
          (unsigned char)(update.downChanged ? 0x01 : 0x00),
          update.repeat,
          update.longpress,
      };

      auto prevDuration = reinterpret_cast<unsigned char *>(&(update.prevDuration));

      payload.insert(
          payload.end(),
          prevDuration,
          prevDuration + sizeof(update.prevDuration));

      payload.insert(
          payload.end(),
          update.pressedMap.begin(),
          update.pressedMap.end());

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("button-event: sending event");
#endif

      udp->event(ids::button, index, payload);
    };

    return handler;
  }

#ifdef IOT_NODE_ESP_NOW_NODE
  Utils::Button::ChangeCallback makeEspNowEvent(unsigned char index)
  {
    auto handler = [index](Utils::Button::Update update)
    {
#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("button-espNow-event: triggered");
#endif

      ::std::vector<unsigned char> payload = {
          ids::button,
          index,
          (update.down ? 0x01 : 0x00),
          (update.downChanged ? 0x01 : 0x00),
          update.repeat,
          update.longpress,
      };

      auto prevDuration = reinterpret_cast<unsigned char *>(&(update.prevDuration));

      payload.insert(
          payload.end(),
          prevDuration,
          prevDuration + sizeof(update.prevDuration));

      payload.insert(
          payload.end(),
          update.pressedMap.begin(),
          update.pressedMap.end());

#ifdef IOT_NODE_LOGGING
      Utils::Log::debug("button-espNow-event: sending event");
#endif

      Utils::EspNowNode::send(payload);
    };

    return handler;
  }
#endif
}

#endif
