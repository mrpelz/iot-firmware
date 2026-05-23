#include "./main.h"

#ifdef IOT_NODE_BUTTONS

namespace IotNode
{
  namespace Events
  {

    namespace Button
    {
      Utils::Button::ChangeCallback makeEvent(Utils::UDP::Class *udp, uint8_t index)
      {
        auto handler = [udp, index](Utils::Button::Update update)
        {
#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("button-event: triggered");
#endif

          std::vector<uint8_t> payload = {
              (uint8_t)(update.down ? 0x01 : 0x00),
              (uint8_t)(update.downChanged ? 0x01 : 0x00),
              update.repeat,
              update.longpress,
          };

          auto prevDuration = reinterpret_cast<uint8_t *>(&(update.prevDuration));

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
      Utils::Button::ChangeCallback makeEspNowEvent(uint8_t index)
      {
        auto handler = [index](Utils::Button::Update update)
        {
#ifdef IOT_NODE_LOGGING
          Utils::Log::debug("button-espNow-event: triggered");
#endif

          std::vector<uint8_t> payload = {
              ids::button,
              index,
              (uint8_t)(update.down ? 0x01 : 0x00),
              (uint8_t)(update.downChanged ? 0x01 : 0x00),
              update.repeat,
              update.longpress,
          };

          auto prevDuration = reinterpret_cast<uint8_t *>(&(update.prevDuration));

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

  } // section namespace
} // project namespace

#endif
