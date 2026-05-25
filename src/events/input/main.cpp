#include "./main.h"

#ifdef IOT_NODE_INPUT

namespace IotNode::Events::Input
{
  Class::Class(unsigned char _pin, bool _pullup, unsigned long _debounceTime, unsigned long _noiseGateTime)
  {
    pin = _pin;
    pullup = _pullup;
    debounceTime = _debounceTime;
    noiseGateTime = _noiseGateTime;
  }

  void Class::setChangeCallback(ChangeCallback callback)
  {
    state.changeCallback = callback;
  }

  void Class::start()
  {
    state.running = true;

    pinMode(
        pin,
        pullup ? INPUT_PULLUP : INPUT);
  }

  void Class::update()
  {
    update(false);
  }
  void Class::update(bool force)
  {
    if (!state.running)
      return;

    auto now = millis();

    auto down = digitalRead(pin);

    if (!force && noiseGateTime)
    {
      if (!state.noiseGateTime && down)
      {
        state.noiseGateTime = now;
        return;
      }

      auto timeSinceNoiseGateBegin = now - state.noiseGateTime;
      if (timeSinceNoiseGateBegin < noiseGateTime && down)
      {
        return;
      }

      if (state.noiseGateTime && !down)
      {
        state.noiseGateTime = 0;
        return;
      }

      state.noiseGateTime = 0;
    }

    auto timeSinceLastChange = now - state.changeTime;
    if (!force && timeSinceLastChange < debounceTime)
      return;

    bool downChanged = down != state.down;

    if (downChanged)
    {
      state.down = down;
    }

    if (!downChanged)
      return;

#ifdef IOT_NODE_LOGGING
    Utils::Log::debug("event: input");
    Utils::Log::debug(fmt::format("input.down: {}", state.down));
#endif

    state.changeCallback(state.down);
  }

  ChangeCallback makeEvent(Utils::UDP::Class *udp, unsigned char index)
  {
    auto handler = [udp, index](bool down)
    {
      udp->event(
          ids::input,
          index,
          {(down ? 0x01 : 0x00)});
    };

    return handler;
  }

#ifdef IOT_NODE_ESP_NOW_NODE
  ChangeCallback makeEspNowEvent(unsigned char index)
  {
    auto handler = [index](bool down)
    {
      ::std::vector<unsigned char> response = {
          ids::input,
          index,
          (down ? 0x01 : 0x00)};

      Utils::EspNowNode::send(response);
    };

    return handler;
  }
#endif
}

#endif
